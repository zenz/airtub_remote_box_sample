#include <Arduino.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#include <ErriezCRC32.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include "tft_handler.h"
#include "ec11_handler.h"
#include "udp_handler.h"

#include "../images/fire.h"
#include "../images/fire_rev.h"

#define LOWER_CASE(x) (                                     \
    {                                                       \
      char *ch = x;                                         \
      while (*ch)                                           \
      {                                                     \
        if (((*ch | 0x20) >= 'a') && ((*ch | 0x20) <= 'z')) \
          *ch |= 0x20;                                      \
        ch++;                                               \
      }                                                     \
    })

WiFiManager wm;
bool shouldSaveConfig = false;
bool configured = false;

#define UDP_PORT 4211

AsyncUDP udp;
IPAddress addr = IPAddress(0, 0, 0, 0);
IPAddress UDP_BCAST_GRP = IPAddress(224, 0, 1, 3);
udp_msg_format_t msg;
bool msg_sent = false;

JsonDocument json;
int target_temp = 0;
int target_mode = 0;
bool modified = false;
unsigned long modified_ts = millis();

struct devices
{
  char dev_name[18];     // Your registered device serial number
  char dev_password[18]; // Your registered device password
  uint8_t dev_mode;      // 0-manual, 1-auto-temp control
  uint8_t dev_active;    // 0-dhw, 1-heating
} devs;

uint8_t CH_MODE = 1;

const char ap_flame_state[] = "fst";
const char ap_fault_state[] = "flt";
bool was_fault = false;

// for DHW
const char dhw_current_mode[] = "cdm";
const char dhw_target_mode[] = "tdm";
const char dhw_current_temp[] = "cdt";
const char dhw_target_temp[] = "tdt";
const int dhw_min_temp = 35;
const int dhw_max_temp = 60;
// for DHW

// for CH
const char ch_current_mode[] = "ccm";
const char ch_target_mode[] = "tcm";
const char ch_current_temp[] = "cct";
const char ch_target_temp[] = "tct";
const int ch_min_temp = 35;
const int ch_max_temp = 80;
// for CH

// for AUTO-ROOM HEATING
const char atm_current_mode[] = "ccm";
const char atm_target_mode[] = "atm";
const char atm_current_temp[] = "crt";
const char atm_target_temp[] = "trt";
const int atm_min_temp = 4;
const int atm_max_temp = 26;
// for AUTO-ROOM HEATING

char *ap_current_mode = nullptr;
char *ap_target_mode = nullptr;
char *ap_current_temp = nullptr;
char *ap_target_temp = nullptr;
int MIN_TEMP = 0;
int MAX_TEMP = 0;

uint8_t target_index = 0;
uint8_t last_dhw_target_mode = 0;
uint8_t last_ch_target_mode = 0;

char target_reply[55];
char my_name[32];

bool show_qrcode = false;
bool qrcode_shown = false;

void change_control_target()
{
  switch (target_index)
  {
  case 0:
    ap_current_mode = (char *)dhw_current_mode;
    ap_target_mode = (char *)dhw_target_mode;
    ap_current_temp = (char *)dhw_current_temp;
    ap_target_temp = (char *)dhw_target_temp;
    MIN_TEMP = dhw_min_temp;
    MAX_TEMP = dhw_max_temp;
    break;

  case 1:
    if (CH_MODE)
    {
      ap_current_mode = (char *)atm_current_mode;
      ap_target_mode = (char *)atm_target_mode;
      ap_current_temp = (char *)atm_current_temp;
      ap_target_temp = (char *)atm_target_temp;
      MIN_TEMP = atm_min_temp;
      MAX_TEMP = atm_max_temp;
    }
    else
    {
      ap_current_mode = (char *)ch_current_mode;
      ap_target_mode = (char *)ch_target_mode;
      ap_current_temp = (char *)ch_current_temp;
      ap_target_temp = (char *)ch_target_temp;
      MIN_TEMP = ch_min_temp;
      MAX_TEMP = ch_max_temp;
    }

    break;

  default:
    break;
  }

  // save running state
  devs.dev_active = target_index;
  EEPROM.begin(512);
  EEPROM.put(0, devs);
  if (EEPROM.commit())
  {
    Serial.println("Settings saved");
  }
  else
  {
    Serial.println("EEPROM error");
  }
  EEPROM.end();

  target_index++;
  if (target_index > 1)
  {
    target_index = 0;
  }

  target_temp = 0;
  target_mode = 0;
}

inline void xor_crypt(uint8_t *buffer, int buf_len, const uint8_t *key, int key_len)
{
  uint8_t *end = buffer + buf_len;
  const uint8_t *k = key;
  for (; buffer != end; ++buffer)
  {
    *buffer ^= *k;
    if (++k - key >= key_len)
    {
      k = key;
    }
  }
}

/* We need to search the Airtub Partner and get it's IP to send data */
bool get_airtub_device_ip()
{
  if (addr == IPAddress(0, 0, 0, 0))
  {
    int n = MDNS.queryService("airtub", "udp");
    if (n > 0)
    {
      for (int i = 0; i < n; ++i)
      {
        if (MDNS.hostname(i) == String(devs.dev_name))
        {
          Serial.printf("Find device %s with IP %s\n", MDNS.hostname(i).c_str(), MDNS.IP(i).toString().c_str());
          addr = MDNS.IP(i);
          return true;
          break;
        }
      }
    }
    else
    {
      Serial.println("No device found!");
      return false;
    }
  }
  return true;
}

void send_udp_msg(udp_msg_format_t msg)
{
  static unsigned long last_sent_ts = millis();
  msg_sent = false;

  /* Do some Airtub Partner communicate treatments first.*/
  Serial.printf("Message Processing: %s\n", msg.data);
  xor_crypt((uint8_t *)msg.data, msg.len, (uint8_t *)devs.dev_password, strlen(devs.dev_password)); // 加密
  msg.crc = crc32Buffer(msg.data, msg.len);

  if (addr != IPAddress(0, 0, 0, 0))
  {
    Serial.println("Sending UDP message");
    do
    {
      if (millis() - last_sent_ts > 1000)
      {
        udp.writeTo((const uint8_t *)&msg, sizeof(msg), addr, UDP_PORT);
        last_sent_ts = millis();
      }
    } while (!msg_sent);
  }
  else
  {
    Serial.println("No device found! Do not send anythong.");
  }
}

void change_value()
{
  char buffer[180];
  udp_msg_format_t msg_send;
  JsonDocument json;
  json["tar"] = devs.dev_name;
  json["dev"] = my_name;
  json[ap_target_temp] = target_temp;
  msg_send.type = 3; // AIRCUBE
  msg_send.len = serializeJson(json, buffer, 180);
  strcpy(msg_send.data, buffer);

  send_udp_msg(msg_send);
}

void change_mode(int mode)
{
  char buffer[180];
  udp_msg_format_t msg_send;
  JsonDocument json;
  json["tar"] = devs.dev_name;
  json["dev"] = my_name;
  json["atm"] = CH_MODE;
  json[ap_target_mode] = mode;
  msg_send.type = 3; // AIRCUBE
  msg_send.len = serializeJson(json, buffer, 180);
  strcpy(msg_send.data, buffer);

  send_udp_msg(msg_send);
}

bool removeSubstring(char *str, const char *toRemove)
{
  char *pos = strstr(str, toRemove);
  if (pos != NULL)
  {
    int len = strlen(toRemove);
    memmove(pos, pos + len, 1 + strlen(pos + len));
    return true;
  }
  return false;
}

void reset_esp()
{
  WiFi.eraseAP();
  EEPROM.begin(512);
  EEPROM.put(0, nullptr);
  EEPROM.commit();
  EEPROM.end();
  ESP.restart();
}

int handleEC11Event()
{
  static int pos = 0;
  static int rotationCount = 0;
  int rotation = 0;

  check_ticks_button(); // Handle button events
  check_ticks_rotary(); // Handle rotary events

  int newPos = encoder->getPosition();
  if (pos != newPos)
  {
    rotationCount++;
    if (rotationCount >= 10)
    {
      rotationCount = 0;
      pos = newPos;
      rotation = (int)(encoder->getDirection());
    }
  }

  switch (buttonState)
  {
  // a negative number to indicate a long pressed time
  case -5:
  case -6:
  case -7:
  case -8:
  case -9:
    Serial.println("Long pressed, restart!");
    reset_esp();
    break;
  case 1:
    if (!show_qrcode)
    {
      Serial.println("Single clicked, change value.");
      modified = false;
      change_value();
    }
    break;
  case 2:
    if (!show_qrcode)
    {
      Serial.println("Double clicked, change mode");
      target_mode = !target_mode;
      change_mode(target_mode);
    }
    break;
  case 3:
    if (!show_qrcode)
    {
      Serial.println("Triple clicked, change control target");
      change_control_target();
    }
    break;
  case 4:
    Serial.println("Triple clicked, switch to show or hide QR code");
    show_qrcode = !show_qrcode;
    if (!show_qrcode)
    {
      qrcode_shown = false;
      tft_clear(0x0000);
    }
    break;
  default:
    break;
  }
  buttonState = 0;
  return rotation;
}

void padWithSpaces(char *str, int totalLength)
{
  int currentLength = strlen(str);
  for (int i = currentLength; i < totalLength; i++)
  {
    str[i] = ' ';
  }
  str[totalLength] = '\0'; // Ensure the string is null-terminated
}

void update_msg_data(const char *json_msg)
{
  DeserializationError error = deserializeJson(json, json_msg);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
  }
}

void udp_callback(AsyncUDPPacket &packet)
{
  if (packet.length() > sizeof(msg))
  { // Incorrect packet length
    Serial.println("Invalid packet length");
    return;
  }
  memcpy(&msg, packet.data(), packet.length());
  if (msg.crc == 0 || msg.type <= 0 || msg.type > 5 || crc32Buffer(msg.data, msg.len) != msg.crc)
  { // Incorrect device type or CRC error.
    Serial.println("Invalid packet");
    return;
  }
  xor_crypt(reinterpret_cast<uint8_t *>(msg.data), msg.len, reinterpret_cast<const uint8_t *>(devs.dev_password), strlen(devs.dev_password)); // decode message
  msg.data[msg.len] = '\0';
  char targetString[30];
  sprintf(targetString, "\"dev\":\"%s\",", devs.dev_name);
  unsigned long time_stamp = millis();
  if (removeSubstring(msg.data, targetString))
  {
    padWithSpaces(msg.data, 180);
    update_msg_data(msg.data);
    Serial.printf("MCAST JSON: %s - %lu\n", json.as<String>().c_str(), time_stamp);
  }
  else if (strstr(msg.data, target_reply) != nullptr)
  {
    Serial.printf("P2P JSON: %s - %lu\n", msg.data, time_stamp);
    msg_sent = true;
  }
  // else
  // {
  //   Serial.printf("Data not for me:%s\n", msg.data);
  // }
}

void saveParamCallback()
{
  shouldSaveConfig = true;
}

void setup(void)
{
  Serial.begin(115200);

  pinMode(PIN_BTN, INPUT_PULLUP); // Must PULLUP first to make button click work correctly
  ec11_init();                    // Initialize button & rotary encoder
  int critical = digitalRead(PIN_BTN);
  if (critical == LOW)
  {
    Serial.println("Reset Remote Box!");
    reset_esp();
  }

  EEPROM.begin(512);
  EEPROM.get(0, devs);
  delay(10);
  EEPROM.end();
  // check if devs is valid
  if (strlen(devs.dev_name) == 0 || strlen(devs.dev_password) == 0)
  {
    Serial.println("Device not configured!");
    configured = false;
  }
  else
  {
    configured = true;
    CH_MODE = devs.dev_mode;
    target_index = devs.dev_active;
    Serial.printf("Device: %s, Password: %s, Mode: %d， Active: %d\n", devs.dev_name, devs.dev_password, devs.dev_mode, devs.dev_active);
  }

  uint64_t chipid = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
  sprintf(my_name, "AIRCUBE-%08X", (uint32_t)chipid);

  tft_init(true);
  Serial.println("Initialized");
  tft_clear(0x0000);
  tft_write(20, 26, 2, "PREPAIRING...", 0xFFE0);

  if (!configured)
  {
    Serial.println("Not configured");
    tft_write(20, 46, 2, "WAITING FOR", 0xFFE0);
    tft_write(20, 66, 2, "CONFIGURATION", 0xFFE0);
    wm.erase(true);
  }
  tft_update();
  delay(10);

  WiFiManagerParameter custom_device("dev", "device serial num:", "", 14, "autocapitalize='none' maxlength=12 required pattern='[a-zA-Z0-9]*' placeholder=\"boiler serial num\"");
  WiFiManagerParameter custom_password("pass", "device password:", "", 14, "maxlength=12 required type=\"password\" placeholder=\"boiler password\"");
  WiFiManagerParameter custom_mode("mode", "heating mode", "", 2, "required type=\"number\" min=\"0\" max=\"1\" placeholder=\"0-manual 1-auto\"");

  wm.addParameter(&custom_device);
  wm.addParameter(&custom_password);
  wm.addParameter(&custom_mode);
  wm.setSaveParamsCallback(saveParamCallback);

  std::vector<const char *> menu = {"wifi"};
  wm.setDebugOutput(true);
  wm.setCountry("CN");
  wm.setHostname(my_name);
  wm.setDarkMode(true);
  wm.setConfigPortalTimeout(180);
  wm.setMenu(menu);
  wm.setConnectTimeout(180);
  bool result = wm.autoConnect(my_name, "4008206306");
  if (!result)
  {
    Serial.println("Failed to setup WiFi");
  }
  else
  {
    if (shouldSaveConfig)
    {
      Serial.println("Should save parameters");
      strcpy(devs.dev_name, custom_device.getValue());
      LOWER_CASE(devs.dev_password); // always lowercase the serial number
      strcpy(devs.dev_password, custom_password.getValue());
      devs.dev_mode = atoi(custom_mode.getValue());
      EEPROM.begin(512);
      EEPROM.put(0, devs);
      if (EEPROM.commit())
      {
        Serial.println("Settings saved");
      }
      else
      {
        Serial.println("EEPROM error");
      }
      EEPROM.end();
      ESP.restart();
    }
  }

  WiFi.mode(WIFI_STA);
  WiFi.enableLongRange(true);
  WiFi.setTxPower(WIFI_POWER_19dBm);
  WiFi.setSleep(false);

  if (mdns_init() != ESP_OK)
  {
    Serial.println("mDNS failed to start");
  }

  sprintf(target_reply, "{\"rec\":1,\"dev\":\"%s\"}", devs.dev_name);
  if (udp.listenMulticast(UDP_BCAST_GRP, UDP_PORT))
  {
    Serial.println("UDP Listening");
    udp.onPacket(udp_callback);
  }
  else
  {
    Serial.println("UDP Listen failed");
  }

  while (!get_airtub_device_ip())
  {
    tft_clear(0x0000);
    tft_write(20, 26, 2, "SEEKING", 0xFFE0);
    tft_write(20, 46, 2, "AIRTUB PARNER", 0xFFE0);
    tft_write(20, 66, 2, "PLEASE WAIT", 0xFFE0);
    tft_update();
    yield();
  };
  change_control_target();
  tft_clear(0x0000);
}

void loop()
{
  static bool fire_fliped = false;
  char buffer[10]; // Make sure this is large enough for your value

  const int rotary = handleEC11Event(); // Handle button and rotary events

  get_airtub_device_ip();

  if (rotary != 0)
  {
    modified = true;
    modified_ts = millis();
  }

  if (millis() - modified_ts > 10000 && modified)
  {
    modified = false;
    target_temp = 0;
  }

  if (show_qrcode)
  {
    if (!qrcode_shown)
    {
      Serial.println("Should show QR code");
      qrcode_shown = true;
      const char *airtub_partner = "http://www.airfit.cn/products/banlu/banlu/170.html";
      tft_gen_qrcode(airtub_partner);
      tft_update();
    }

    return;
  }

  // handling error code
  if (json.containsKey(ap_fault_state))
  {
    int value_flt = json[ap_fault_state].as<int>();
    if (value_flt > 0)
    {
      was_fault = true;
      sprintf(buffer, "E%02D", value_flt);
      tft_clear(0x0000);
      tft_write(20, 26, 12, buffer, 0xF800);
      tft_update();
      return;
    }
  }
  else if (was_fault)
  {
    was_fault = false;
    tft_clear(0x0000);
  }

  // handling current temp
  int value_temp = 0;
  if (json.containsKey(ap_current_temp))
  {
    if (target_index == 0)
    {
      // auto heating mode
      float value_temp_float = json[ap_current_temp].as<float>();
      value_temp = round(value_temp_float - 0.1);
    }
    else
    {
      // water or manual heating mode
      value_temp = json[ap_current_temp].as<int>();
    }
    sprintf(buffer, "%02d", value_temp);
    const char *value = buffer;
    tft_write(15, 26, 12, value, 0xFFE0);
  }

  // handling target temp
  int value_target_temp = 0;
  if (json.containsKey(ap_target_temp))
  {
    value_target_temp = json[ap_target_temp].as<int>();
  }

  // handle target mode
  if (json.containsKey(ap_target_mode))
  {
    target_mode = json[ap_target_mode].as<int>();
  }

  if (target_temp == 0 && value_target_temp != 0)
  {
    target_temp = value_target_temp;
  }

  target_temp += rotary;
  target_temp = constrain(target_temp, MIN_TEMP, MAX_TEMP); // Limit target temperature
  if (target_temp != value_target_temp && modified)
  {
    sprintf(buffer, "%02d", target_temp);
    const char *value = buffer;
    // Serial.printf("Local Settings: %s\n", value);
    tft_write(160, 70, 6, value, 0xF800);
  }
  else
  {
    sprintf(buffer, "%02d", value_target_temp);
    const char *value = buffer;
    // Serial.printf("tct: %s\n", value);
    tft_write(160, 70, 6, value, 0x5D1C);
  }

  int current_mode = 0;
  if (json.containsKey(ap_current_mode))
  {
    current_mode = json[ap_current_mode].as<int>();
  }

  if (json.containsKey(ap_flame_state))
  {
    static unsigned long last_flip = millis();
    int value_fst = json[ap_flame_state].as<int>();
    if (value_fst == 1 && current_mode == 1)
    {
      if (millis() - last_flip > 500)
      {
        last_flip = millis();
        fire_fliped = !fire_fliped;

        if (fire_fliped == false)
        {
          tft_draw_bitmap(155, 12, fire_rev, 48, 48);
        }
        else
        {
          tft_draw_bitmap(155, 12, fire, 48, 48);
        }
      }
    }
    else
    {
      tft_clear_rect(155, 12, 48, 48, 0x0000);
      tft_draw_round_rect(160, 60, 38, 5, 5, 0xB5B6);
    }
  }

  // draw the switch
  tft_draw_round_rect(209, 20, 16, 40, 5, target_mode ? 0x0F80 : 0xBDF7);
  if (target_mode)
  {
    tft_draw_round_rect(209, 20, 16, 22, 5, 0x0464);
  }
  else
  {
    tft_draw_round_rect(209, 38, 16, 22, 5, 0x7BCF);
  }
  tft_write_transperant(212, 24, 2, target_index == 1 ? "W" : "H", 0xFFFF);
  tft_update();
}
