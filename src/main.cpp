#include <Arduino.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#include <ErriezCRC32.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include "tft_handler.h"
#include "ec11.h"
#include "udp_handler.h"
#include "string.h"

#include "../images/fire.h"

#define UDP_PORT 4211

AsyncUDP udp;
IPAddress addr = IPAddress(0, 0, 0, 0);
IPAddress UDP_BCAST_GRP = IPAddress(224, 0, 1, 3);
udp_msg_format_t msg;

JsonDocument json;
int target_dhw_temp = 0;
int target_dhw_mode = 0;
bool modified = false;
unsigned long modified_ts = millis();

char *wifi_ssid = "YOUR WIFI SSID";            // Your WiFi SSID
char *wifi_password = "YOUR WIFI PASSWORD";    // Your WiFi password
char *dev_name = "YOUR BOILER SERIAL NUMBER";  // Your registered device serial number
char *dev_password = "YOUR REGISTER PASSWORD"; // Your registered device password

char target_reply[55];
char my_name[32];

bool show_qrcode = false;
bool qrcode_shown = false;

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
void get_airtub_device_ip()
{
  if (addr == IPAddress(0, 0, 0, 0))
  {
    int n = MDNS.queryService("airtub", "udp");
    if (n > 0)
    {
      for (int i = 0; i < n; ++i)
      {
        if (MDNS.hostname(i) == String(dev_name))
        {
          Serial.printf("Find device %s with IP %s\n", MDNS.hostname(i).c_str(), MDNS.IP(i).toString().c_str());
          addr = MDNS.IP(i);
          break;
        }
      }
    }
    else
    {
      Serial.println("No device found!");
    }
  }
}

void send_udp_msg(udp_msg_format_t msg)
{
  /* Do some Airtub Partner communicate treatments first.*/
  Serial.printf("Message Processing: %s\n", msg.data);
  xor_crypt((uint8_t *)msg.data, msg.len, (uint8_t *)dev_password, strlen(dev_password)); // 加密
  msg.crc = crc32Buffer(msg.data, msg.len);

  if (addr != IPAddress(0, 0, 0, 0))
  {
    Serial.println("Sending UDP message");
    udp.writeTo((const uint8_t *)&msg, sizeof(msg), addr, UDP_PORT);
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
  json["tar"] = dev_name;
  json["dev"] = my_name;
  json["tdt"] = target_dhw_temp;
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
  json["tar"] = dev_name;
  json["dev"] = my_name;
  json["tdm"] = mode;
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
    if (rotationCount >= 40)
    {
      rotationCount = 0;
      pos = newPos;
      rotation = (int)(encoder->getDirection());
    }
  }

  switch (buttonState)
  {
  // 得到负数，为长按，点击为整数
  case -5:
  case -6:
  case -7:
  case -8:
  case -9:
    Serial.println("Long pressed, restart!");
    ESP.restart();
    break;
  case 1:
    Serial.println("Single clicked, change value.");
    modified = false;
    change_value();
    break;
  case 2:
    Serial.println("Double clicked, change mode");
    target_dhw_mode = !target_dhw_mode;
    change_mode(target_dhw_mode);
    break;
  case 3:
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
  xor_crypt(reinterpret_cast<uint8_t *>(msg.data), msg.len, reinterpret_cast<const uint8_t *>(dev_password), strlen(dev_password)); // decode message
  msg.data[msg.len] = '\0';
  char targetString[30];
  sprintf(targetString, "\"dev\":\"%s\",", dev_name);
  if (removeSubstring(msg.data, targetString))
  {
    padWithSpaces(msg.data, 180);
    update_msg_data(msg.data);
    Serial.printf("JSON: %s\n", json.as<String>().c_str());
  }
  else if (strstr(msg.data, target_reply) != nullptr)
  {
    Serial.printf("Airtub Partner reply: %s\n", msg.data);
  }
  // else
  // {
  //   Serial.printf("Data not for me:%s\n", msg.data);
  // }
}

void setup(void)
{
  Serial.begin(115200);

  pinMode(PIN_BTN, INPUT_PULLUP); // Must PULLUP first to make button click work correctly
  ec11_init();                    // Initialize button & rotary encoder

  char AutoConnectAP[32];
  uint64_t chipid = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
  sprintf(my_name, "AIRCUBE-%08X", (uint32_t)chipid);

  tft_init(true);
  Serial.println("Initialized");
  tft_clear(0x0000);
  tft_write(20, 26, 2, "PREPAIRING...", 0xFFE0);

  WiFi.begin(wifi_ssid, wifi_password);
  WiFi.hostname(my_name);

  if (mdns_init() != ESP_OK)
  {
    Serial.println("mDNS failed to start");
  }

  sprintf(target_reply, "{\"rec\":1,\"dev\":\"%s\"}", dev_name);
  if (udp.listenMulticast(UDP_BCAST_GRP, UDP_PORT))
  {
    Serial.println("UDP Listening");
    udp.onPacket(udp_callback);
  }
  else
  {
    Serial.println("UDP Listen failed");
  }

  get_airtub_device_ip();
  tft_clear(0x0000);
}

void loop()
{
  static bool fire_fliped = false;
  char buffer[10];        // Make sure this is large enough for your value
  get_airtub_device_ip(); // always search the Airtub Partner if not get one.

  const int rotary = handleEC11Event(); // Handle button and rotary events
  if (rotary != 0)
  {
    modified = true;
    modified_ts = millis();
  }

  if (millis() - modified_ts > 10000 && modified)
  {
    modified = false;
  }

  if (show_qrcode)
  {
    if (!qrcode_shown)
    {
      Serial.println("Should show QR code");
      qrcode_shown = true;
      const char *airtub_partner = "http://www.airfit.cn/products/banlu/banlu/170.html";
      tft_gen_qrcode(airtub_partner);
    }

    return;
  }

  int value_cdt = 0;
  if (json.containsKey("cdt"))
  {
    value_cdt = json["cdt"].as<int>();
    sprintf(buffer, "%d", value_cdt);
    const char *value = buffer;
    tft_write(15, 26, 12, value, 0xFFE0);
  }

  int value_tdt = 0;
  if (json.containsKey("tdt"))
  {
    value_tdt = json["tdt"].as<int>();
  }

  if (json.containsKey("tdm"))
  {
    target_dhw_mode = json["tdm"].as<int>();
  }

  if (target_dhw_temp == 0 && value_tdt != 0)
  {
    target_dhw_temp = value_tdt;
    Serial.printf("Set target_dhw_temp to %d\n", target_dhw_temp);
  }

  target_dhw_temp += rotary;
  if (target_dhw_temp != 0)
  {
    target_dhw_temp = constrain(target_dhw_temp, 35, 60); // Limit DHW temperature
    if (target_dhw_temp != value_tdt && modified)
    {
      sprintf(buffer, "%d", target_dhw_temp);
      const char *value = buffer;
      // Serial.printf("Local Settings: %s\n", value);
      tft_write(160, 70, 6, value, 0xF800);
    }
    else
    {
      sprintf(buffer, "%d", value_tdt);
      const char *value = buffer;
      // Serial.printf("tct: %s\n", value);
      tft_write(160, 70, 6, value, 0x5D1C);
    }
  }

  int current_dhw_mode = 0;
  if (json.containsKey("cdm"))
  {
    current_dhw_mode = json["cdm"].as<int>();
  }

  if (json.containsKey("fst"))
  {
    static unsigned long last_flip = millis();
    int value_fst = json["fst"].as<int>();
    if (value_fst == 1 && current_dhw_mode == 1)
    {
      if (millis() - last_flip > 500)
      {
        last_flip = millis();
        fire_fliped = !fire_fliped;

        if (fire_fliped == false)
        {
          tft_draw_bitmap_reverse(155, 12, fire, 48, 48);
        }
        else
        {
          tft_draw_bitmap(155, 12, fire, 48, 48);
        }
      }
    }
    else
    {
      tft_clear_rect(155, 12, 48, 49, 0x0000);
    }
  }
  tft_draw_round_rect(210, 20, 15, 40, 5, target_dhw_mode ? 0x0F80 : 0xF800);
}
