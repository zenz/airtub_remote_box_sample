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
#include "time.h"

#include "../assets/fire_small.h"
#include "../assets/fire_small_rev.h"
#include "../assets/fire_normal.h"
#include "../assets/fire_normal_rev.h"
#include "../assets/fire_large.h"
#include "../assets/fire_large_rev.h"
#include "../assets/plug.h"
#include "../assets/battery.h"
#include "../assets/wifi_good.h"
#include "../assets/wifi_bad.h"
#include "../assets/wifi_normal.h"
#include "../assets/shower.h"
#include "../assets/shower_burn.h"
#include "../assets/radiator.h"
#include "../assets/radiator_burn.h"
#include "../assets/room.h"
#include "../assets/room_ext.h"
#include "../assets/outdoor.h"
#include "../assets/seeking.h"
#ifdef USE_CHINESE
#include "../assets/configure.h"
#include "../assets/operate0.h"
#include "../assets/operate1.h"
#include "../assets/ap_seeking.h"
#include "../assets/connecting.h"
#include "../assets/erasing.h"
#endif

#ifdef DEBUG_ENABLED
#define DebugBegin(...) ({ Serial.begin(__VA_ARGS__); })
#define DebugPrint(...) ({ Serial.print(__VA_ARGS__); })
#define DebugPrintln(...) ({ Serial.println(__VA_ARGS__); })
#define DebugPrintf(...) ({ Serial.printf(__VA_ARGS__); })
#define DebugWrite(...) ({ Serial.write(__VA_ARGS__); })
#else
#define DebugBegin(...)
#define DebugPrint(...)
#define DebugPrintln(...)
#define DebugPrintf(...)
#define DebugWrite(...)
#endif // DEBUG_ENABLED

#ifdef BOARD_V1
#define PIN_RECONFIG 21
#else
#define PIN_RECONFIG 0
#endif

#ifdef USE_DEEP_SLEEP
#define WAKEUP_PIN GPIO_NUM_18
#define WAKEUP_LEVEL LOW
RTC_DATA_ATTR uint8_t target_index = 0;
#ifndef DEEPSLEEP_TIME
#define DEEPSLEEP_TIME 30000 // 30 seconds
#endif
#else
uint8_t target_index = 0;
#endif

#define SEND_TIMEOUT 10000   // 10 seconds
#define MODIFY_TIMEOUT 10000 // 10 seconds

#ifdef BOARD_V1
#define POWER_CABLE_PIN 40
#else
#define POWER_CABLE_PIN 39
#endif

#ifdef POWER_DETECT
uint8_t power = 0;
#else
uint8_t power = 1;
#endif

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

bool shouldSaveConfig = false;
bool configured = false;

#define UDP_PORT 4211

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 28800; // 8 hours
const int daylightOffset_sec = 0;

WiFiManager wm;
const char *my_password = "4008206306";
AsyncUDP udp;
IPAddress addr = IPAddress(0, 0, 0, 0);
IPAddress UDP_BCAST_GRP = IPAddress(224, 0, 1, 3);
udp_msg_format_t msg;
bool msg_sent = false;
unsigned long last_mcast_ts = millis();

JsonDocument json;
int target_temp = 0;
int target_mode = 0;
bool modified = false;
unsigned long modified_ts = millis();
unsigned long active_ts = millis();

struct devices
{
  char dev_name[18];     // Your registered device serial number
  char dev_password[18]; // Your registered device password
  char dev_ssid[32];     // Your WiFi SSID
  uint8_t dev_mode;      // 0-manual, 1-auto-temp control
} devs;

uint8_t CH_MODE = 1;

const char over_temp[] = "ovr";
const char ext_temp_pwr[] = "pwr";
const char out_door_temp[] = "odt";
const char target_device[] = "tar";
const char sender_device[] = "dev";
const char reset_command[] = "clr";
const char update_state[] = "sta";

const char ap_flame_state[] = "fst";
const char ap_fault_state[] = "flt";
const char ap_modulate[] = "mod";
int fire_modulate = 0;
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

uint8_t last_dhw_target_mode = 0;
uint8_t last_ch_target_mode = 0;

char target_reply[55];
char my_name[32];

bool show_qrcode = false;
bool qrcode_shown = false;

// Get WiFi Signal Quality
int wifi_get_rssi()
{
  int8_t rssi = WiFi.RSSI();

  if (rssi <= -100)
  {
    return 0;
  }
  else if (rssi >= -50)
  {
    return 100;
  }
  else
  {
    return 2 * (rssi + 100);
  }
}

void switch_mode(uint8_t mode)
{
  if (mode)
  {
    ap_current_mode = (char *)atm_current_mode;
    ap_target_mode = (char *)atm_target_mode;
    ap_current_temp = (char *)atm_current_temp;
    ap_target_temp = (char *)atm_target_temp;
    target_mode = 1; // in auto mode, target mode is always on
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
}

void change_control_target(const uint8_t target_index)
{
  if (target_index == 0)
  {
    ap_current_mode = (char *)dhw_current_mode;
    ap_target_mode = (char *)dhw_target_mode;
    ap_current_temp = (char *)dhw_current_temp;
    ap_target_temp = (char *)dhw_target_temp;
    MIN_TEMP = dhw_min_temp;
    MAX_TEMP = dhw_max_temp;
  }
  else
  {
    switch_mode(CH_MODE);
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

void send_udp_msg(udp_msg_format_t msg)
{
  static unsigned long last_sent_ts = millis();
  msg_sent = false;

  /* Do some Airtub Partner communicate treatments first.*/
  DebugPrintf("Message Processing: %s\n", msg.data);
  xor_crypt((uint8_t *)msg.data, msg.len, (uint8_t *)devs.dev_password, strlen(devs.dev_password)); // 加密
  msg.crc = crc32Buffer(msg.data, msg.len);

  if (addr == IPAddress(0, 0, 0, 0))
  {
    DebugPrintln("No device found! Do not send anything.");
    return;
  }

  DebugPrintln("Sending UDP message");
  unsigned long start_ts = millis();
  do
  {
    yield(); // always give time to handle other tasks
    if (millis() - last_sent_ts > 1000)
    {
      udp.writeTo((const uint8_t *)&msg, sizeof(msg), addr, UDP_PORT);
      last_sent_ts = millis();
    }
    // 超时退出循环
    if (millis() - start_ts > SEND_TIMEOUT)
    {
      DebugPrintln("Send message timeout!");
      addr = IPAddress(0, 0, 0, 0);
      break;
    }
  } while (!msg_sent);
}

// change target temp
void change_value()
{
  char buffer[180];
  udp_msg_format_t msg_send;
  JsonDocument json;
  json[target_device] = devs.dev_name;
  json[sender_device] = my_name;
  json[ap_target_temp] = target_temp;
  json[update_state] = 1;
  msg_send.type = 3; // AIRCUBE
  msg_send.len = serializeJson(json, buffer, 180);
  strcpy(msg_send.data, buffer);

  send_udp_msg(msg_send);
}

// change target mode
void change_mode(uint8_t mode)
{
  bool not_auto_mode = false;
  char buffer[180];
  JsonDocument json;

  json[target_device] = devs.dev_name;
  json[sender_device] = my_name;
  json[update_state] = 1;
  if (strcmp(ap_target_mode, atm_target_mode) != 0)
  {
    not_auto_mode = true;
    json[ap_target_mode] = mode;
  }
  if (not_auto_mode)
  {
    udp_msg_format_t msg_send;
    msg_send.type = 3; // AIRCUBE
    msg_send.len = serializeJson(json, buffer, 180);
    strcpy(msg_send.data, buffer);

    send_udp_msg(msg_send);
  }
}

void switch_heating_mode(uint8_t mode)
{
  char buffer[180];
  udp_msg_format_t msg_send;
  JsonDocument json;

  json[target_device] = devs.dev_name;
  json[sender_device] = my_name;
  if (mode)
  {
    json[atm_target_mode] = 1;
  }
  else
  {
    json[atm_target_mode] = 0;
    json[ch_target_temp] = 35;
  }
  json[update_state] = 1;
  msg_send.type = 3; // AIRCUBE
  msg_send.len = serializeJson(json, buffer, 180);
  strcpy(msg_send.data, buffer);

  send_udp_msg(msg_send);
}

// reset error
void reset_error()
{
  char buffer[180];
  udp_msg_format_t msg_send;
  JsonDocument json;
  json[target_device] = devs.dev_name;
  json[sender_device] = my_name;
  json[reset_command] = 1;
  msg_send.type = 3; // AIRCUBE
  msg_send.len = serializeJson(json, buffer, 180);
  strcpy(msg_send.data, buffer);

  send_udp_msg(msg_send);
}

// remove specified substring
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

void save_config()
{
  EEPROM.begin(512);
  EEPROM.put(0, devs);
  if (EEPROM.commit())
  {
    DebugPrintln("Settings saved");
  }
  else
  {
    DebugPrintln("EEPROM error");
  }
  EEPROM.end();
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
    active_ts = millis();
  }

  if (buttonState != 0)
  {
    active_ts = millis();
  }

  switch (buttonState)
  {
  case -2:
  case -3:
  case -4:
  case -5:
  case -6:
    if (!show_qrcode && target_index == 1)
    {
      DebugPrintln("Long pressed, change heating manage mode.");
      CH_MODE = !CH_MODE;
      switch_mode(CH_MODE);
      switch_heating_mode(CH_MODE);
      devs.dev_mode = CH_MODE;
      save_config();
    }
    break;
  case 1:
    if (show_qrcode)
    {
      show_qrcode = false;
      qrcode_shown = false;
      tft_clear(BLACK);
    }
    else if (was_fault)
    {
      DebugPrintln("Was fault and single clicked, reset error.");
      reset_error();
    }
    else
    {
      DebugPrintln("Single clicked, change value.");
      modified = false;
      change_value();
    }
    break;
  case 2:
    if (!show_qrcode)
    {
      DebugPrintln("Double clicked, change mode");
      target_mode = !target_mode;
      change_mode(target_mode);
    }
    break;
  case 3:
    if (!show_qrcode)
    {
      DebugPrintln("Triple clicked, change control target");
      target_index = (target_index + 1) % 2;
      change_control_target(target_index);
    }
    break;
  case 4:
    DebugPrintln("Triple clicked, switch to show or hide QR code");
    show_qrcode = true;
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
    DebugPrint(F("deserializeJson() failed: "));
    DebugPrintln(error.c_str());
  }
}

void udp_callback(AsyncUDPPacket &packet)
{
  if (packet.length() > sizeof(msg))
  { // Incorrect packet length
    DebugPrintln("Invalid packet length");
    return;
  }
  memcpy(&msg, packet.data(), packet.length());
  if (msg.crc == 0 || msg.type <= 0 || msg.type > 5 || crc32Buffer(msg.data, msg.len) != msg.crc)
  { // Incorrect device type or CRC error.
    DebugPrintln("Invalid packet");
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
    DebugPrintf("MCAST JSON: %s - %lu\n", json.as<String>().c_str(), time_stamp);
    last_mcast_ts = millis();
    if (addr == IPAddress(0, 0, 0, 0))
    {
      // 从广播信息中也可以获取Airtub Partner的IP
      addr = packet.remoteIP();
      DebugPrintf("Find airtub partner from udp_callback: %s\n", addr.toString().c_str());
    }
  }
  else if (strstr(msg.data, target_reply) != nullptr)
  {
    DebugPrintf("P2P JSON: %s - %lu\n", msg.data, time_stamp);
    msg_sent = true;
  }
  // else
  // {
  //   DebugPrintf("Data not for me:%s\n", msg.data);
  // }
}

String getParam(String name)
{
  // read parameter from server, for customhmtl input
  String value;
  if (wm.server->hasArg(name))
  {
    value = wm.server->arg(name);
  }
  return value;
}

void showWiFiQRCode(WiFiManager *wm)
{
  char wifi_code[50];
  sprintf(wifi_code, "WIFI:T:WPA;S:%s;P:%s;;", my_name, my_password);
  tft_gen_qrcode(wifi_code, DARK_GREEN, BLACK);
  tft_update();
}

// callback for WiFiManager to save parameters
void saveParamCallback()
{
  shouldSaveConfig = true;
  devs.dev_mode = getParam("heatmodeid").toInt();
}

void deep_sleep_now()
{
#ifdef USE_DEEP_SLEEP
  esp_sleep_enable_ext0_wakeup(WAKEUP_PIN, WAKEUP_LEVEL);
  esp_deep_sleep_start();
#endif
}

void displayLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    DebugPrintln("Failed to obtain time");
    return;
  }
  char time_str[30];
  strftime(time_str, 30, "%H:%M", &timeinfo);
  // DebugPrintf("Local Time: %s\n", time_str);
  tft_write(80, 6, 2, time_str, WHITE);
}

void setup(void)
{
  DebugBegin(115200);

  pinMode(PIN_RECONFIG, INPUT_PULLUP); // for device reconfiguration

  pinMode(PIN_BTN, INPUT_PULLUP); // Must PULLUP first to make button click work correctly
  ec11_init();                    // Initialize button & rotary encoder

#ifdef POWER_DETECT
  pinMode(POWER_CABLE_PIN, INPUT);
  power = digitalRead(POWER_CABLE_PIN);
#endif

  EEPROM.begin(512);
  EEPROM.get(0, devs);
  delay(10);
  EEPROM.end();
  // check if devs is valid
  if (strlen(devs.dev_name) == 0 || strlen(devs.dev_password) == 0)
  {
    DebugPrintln("Device not configured!");
    configured = false;
  }
  else
  {
    configured = true;
    CH_MODE = devs.dev_mode;
    DebugPrintf("Device: %s, Password: %s, Mode: %d， Active: %d\n", devs.dev_name, devs.dev_password, devs.dev_mode, target_index);
  }

  uint64_t chipid = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
  sprintf(my_name, "AIRCUBE-%08X", (uint32_t)chipid);

  tft_init(true);
  DebugPrintln("Display ready.");

  if (power == HIGH)
  {
    DebugPrintln("CABLE_PLUGINED!");
  }

  int critical = digitalRead(PIN_RECONFIG);
  if (critical == LOW)
  {
    DebugPrintln("Reset Remote Box!");
    wm.erase(true); // erase all saved settings
    tft_clear(BLACK);
#ifdef USE_CHINESE
    tft_draw_bitmap(20, 26, erasing, 116, 24);
#else
    tft_write(20, 26, 2, "ERASING...", YELLOW);
#endif
    tft_update();
    delay(500);
    reset_esp();
  }

  if (!configured)
  {
    DebugPrintln("Not configured");
    tft_clear(BLACK);
#ifdef USE_CHINESE
    tft_draw_bitmap(20, 26, configure, 123, 72);
#else
    tft_write(20, 26, 2, "NEW DEVICE", YELLOW);
    tft_write(20, 46, 2, "WAITING FOR", YELLOW);
    tft_write(20, 66, 2, "CONFIGURATION", YELLOW);
#endif
  }
  else
  {
    tft_clear(BLACK);
#ifdef USE_CHINESE
    tft_draw_bitmap(20, 26, connecting, 169, 48);
    tft_write(22, 78, 2, devs.dev_ssid, YELLOW);
#else
    tft_write(20, 26, 2, "PREPAIRING...", YELLOW);
    tft_write(20, 46, 2, "CONNECT WIFI", YELLOW);
    tft_write(20, 66, 2, devs.dev_ssid, YELLOW);
#endif
  }
  tft_update();
  delay(10);

  WiFi.setSleep(false);

  if (!configured)
  {
    std::vector<const char *> menu = {"wifi"};
    wm.setDebugOutput(true);
    wm.setCountry("CN");
    wm.setHostname(my_name);
    wm.setDarkMode(true);
    wm.setConfigPortalTimeout(180);
    wm.setMenu(menu);
    wm.setConnectTimeout(180);
    wm.setMinimumSignalQuality(20);
    wm.setAPStaticIPConfig(IPAddress(10, 0, 1, 1), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));

    WiFiManagerParameter custom_device("dev", "设备序列号:", "", 14, "autocapitalize='none' maxlength=12 required pattern='[a-zA-Z0-9]*' placeholder=\"壁挂炉序列号\"");
    WiFiManagerParameter custom_password("pass", "设备密码:", "", 14, "maxlength=12 required type=\"password\" placeholder=\"设备密码\"");
    // WiFiManagerParameter custom_mode("mode", "heating mode", "", 2, "required type=\"number\" min=\"0\" max=\"1\" placeholder=\"0-manual 1-auto\"");
    WiFiManagerParameter custom_field;
    const char *custom_radio_str = "<br/><label for='heatmodeid'>采暖模式</label><br/><input type='radio' name='heatmodeid' value='0'> 手动模式<br/><input type='radio' name='heatmodeid' value='1' checked> 自动模式<br/>";
    new (&custom_field) WiFiManagerParameter(custom_radio_str);

    wm.addParameter(&custom_device);
    wm.addParameter(&custom_password);
    // wm.addParameter(&custom_mode);
    wm.addParameter(&custom_field);
    wm.setSaveParamsCallback(saveParamCallback);
    wm.setAPCallback(showWiFiQRCode);

    bool result = wm.autoConnect(my_name, my_password);
    if (!result)
    {
      DebugPrintln("Failed to setup WiFi");
#ifdef USE_DEEP_SLEEP
      deep_sleep_now();
#endif
    }
    else
    {
      if (shouldSaveConfig)
      {
        DebugPrintln("Should save parameters");
        strcpy(devs.dev_name, custom_device.getValue());
        LOWER_CASE(devs.dev_name); // always lowercase the serial number
        strcpy(devs.dev_password, custom_password.getValue());
        // devs.dev_mode = atoi(custom_mode.getValue());
        strncpy(devs.dev_ssid, WiFi.SSID().c_str(), sizeof(devs.dev_ssid));
        devs.dev_ssid[sizeof(devs.dev_ssid) - 1] = '\0'; // 确保字符串以 null 结尾
        save_config();
        ESP.restart();
      }
    }
  }
  else
  {
    // already configured
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(my_name);
    WiFi.setTxPower(WIFI_POWER_17dBm);
    WiFi.enableLongRange(1);
    WiFi.begin();
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      DebugPrint(".");
      if (millis() - active_ts > DEEPSLEEP_TIME)
      {
        deep_sleep_now(); // 超过设定时间未连上WiFi，进入休眠。
      }
    }
    DebugPrintln("");
  }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  // displayLocalTime();

  if (mdns_init() != ESP_OK)
  {
    DebugPrintln("mDNS failed to start");
  }

  sprintf(target_reply, "{\"rec\":1,\"dev\":\"%s\"}", devs.dev_name);
  if (udp.listenMulticast(UDP_BCAST_GRP, UDP_PORT))
  {
    DebugPrintln("UDP Listening");
    udp.onPacket(udp_callback);
  }
  else
  {
    DebugPrintln("UDP Listen failed");
  }

  // check wake up reason and set target_index
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason != ESP_SLEEP_WAKEUP_EXT0)
  {
    target_index = 0;
  }

  String devname = devs.dev_name;
  devname.toUpperCase();
  tft_clear(BLACK);
  change_control_target(target_index);
  tft_clear(BLACK);
}

void loop()
{
  static bool airtub_seeking = true;
  static bool fire_fliped = false;
  static unsigned long wifi_rssi_last_ts = 0;
  static unsigned long flame_blink_ts = 0;
  static uint8_t currentOperateHint = 0;
  static bool initialized = false;

  uint8_t fire_working_mode = 0;
  uint8_t current_mode = 0;
  int outdoor_length = 0;

  // #ifdef DEBUG_ENABLED
  //   const unsigned long start_time = millis();
  // #endif

#ifdef POWER_DETECT
  power = digitalRead(POWER_CABLE_PIN);
#endif
  if (power == HIGH)
  {
    tft_draw_bitmap(10, 5, plug, 25, 16);
    active_ts = millis();
  }
  else
  {
    tft_draw_bitmap(10, 5, battery, 25, 16);
  }

  char buffer[10]; // Make sure this is large enough for your value

  const int rotary = handleEC11Event(); // Handle button and rotary events

  if (rotary != 0)
  {
    modified = true;
    modified_ts = millis();
    active_ts = millis();
  }

  if (millis() - modified_ts > MODIFY_TIMEOUT && modified)
  {
    modified = false;
    target_temp = 0;
  }
  else if (!modified)
  {
    target_temp = json[ap_target_temp].as<int>();
  }

  if (show_qrcode)
  {
    if (!qrcode_shown)
    {
      DebugPrintln("Should show QR code");
      qrcode_shown = true;
      const char *airtub_partner = "http://www.airfit.cn/products/banlu/banlu/170.html";
      tft_gen_qrcode(airtub_partner, DARK_GREEN, BLACK);
      tft_update();
    }
    if (millis() - active_ts > DEEPSLEEP_TIME && power == LOW)
    {
      deep_sleep_now();
    }

    return;
  }

#ifndef WIFI_SIGNAL_UPDATE_TIME
#define WIFI_SIGNAL_UPDATE_TIME 10000
#endif
  if (millis() - wifi_rssi_last_ts > WIFI_SIGNAL_UPDATE_TIME)
  {
    const int wifi_rssi = wifi_get_rssi();
    if (wifi_rssi > 70)
    {
      tft_draw_bitmap(40, 5, wifi_good, 16, 16);
    }
    else if (wifi_rssi > 30)
    {
      tft_draw_bitmap(40, 5, wifi_normal, 16, 16);
    }
    else
    {
      tft_draw_bitmap(40, 5, wifi_bad, 16, 16);
    }
    // wifi_rssi_last_ts = millis(); // 更新放到操作提示后，重复利用
  }

  displayLocalTime();

  if (json.containsKey(atm_target_mode))
  {
    const uint8_t target_auto_mode = json[atm_target_mode].as<int>();
    if (target_auto_mode != CH_MODE)
    {
      CH_MODE = target_auto_mode;
      switch_mode(CH_MODE);
      if (CH_MODE)
        target_temp = json[atm_target_temp].as<int>();
      else
        target_temp = json[ch_target_temp].as<int>();
    }
  }

  if (target_index == 0)
  {
    tft_draw_bitmap(60, 5, shower, 16, 16);
  }
  else
  {
    if (CH_MODE)
    {
      if (json.containsKey(ext_temp_pwr))
      {
        tft_draw_bitmap(60, 5, room_ext, 16, 16);
      }
      else
      {
        tft_draw_bitmap(60, 5, room, 16, 16);
      }
    }
    else
    {
      tft_draw_bitmap(60, 5, radiator, 16, 16);
    }
  }

  // handling outdooor temp
  if (json.containsKey(out_door_temp))
  {
    int value_odt = json[out_door_temp].as<int>();
    sprintf(buffer, "%02d", value_odt);
    const char *value = buffer;
    outdoor_length = strlen(value);
    tft_draw_bitmap(outdoor_length > 2 ? 170 : 182, 5, outdoor, 16, 16);
    tft_write(outdoor_length > 2 ? 190 : 202, 5, 2, value, WHITE);
  }
  else
  {
    tft_clear_rect(150, 5, 90, 16, BLACK); // also need to clear the rect for flash icon
  }

  // handling error code
  if (json.containsKey(ap_fault_state))
  {
    int value_flt = json[ap_fault_state].as<int>();
    if (value_flt > 0)
    {
      was_fault = true;
      sprintf(buffer, "E%02D", value_flt);
      tft_clear(BLACK);
      tft_write(20, 26, 12, buffer, RED);
      tft_update();
      return;
    }
  }
  else if (was_fault)
  {
    was_fault = false;
    tft_clear(BLACK);
  }

  // handling current temp
  int value_temp = 0;
  if (json.containsKey(ap_current_temp))
  {
    float value_temp_float = json[ap_current_temp].as<float>();
    value_temp = round(value_temp_float - 0.1);
    sprintf(buffer, "%02d", value_temp);
    const char *value = buffer;
    if (json.containsKey(over_temp) && json[over_temp].as<bool>() == true)
    {
      tft_write(15, 26, 12, value, RED);
    }
    else
    {
      tft_write(15, 26, 12, value, YELLOW);
    }
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
    // DebugPrintf("Local Settings: %s\n", value);
    tft_write(160, 70, 6, value, RED);
  }
  else
  {
    sprintf(buffer, "%02d", value_target_temp);
    const char *value = buffer;
    // DebugPrintf("tct: %s\n", value);
    tft_write(160, 70, 6, value, BLUE);
  }

  if (json.containsKey(ap_current_mode))
  {
    current_mode = json[ap_current_mode].as<int>();
  }

  if (json.containsKey(ap_modulate))
  {
    fire_modulate = json[ap_modulate].as<int>();
  }

  if ((millis() / 1000) % 2 == 0)
  {
    fire_working_mode = 0;
  }
  else if (json.containsKey(dhw_current_mode) && json[dhw_current_mode].as<int>() == 1)
  {
    fire_working_mode = 1;
  }
  else if (json.containsKey(ch_current_mode) && json[ch_current_mode].as<int>() == 1)
  {
    fire_working_mode = 2;
  }

  // draw the flame
  if (json.containsKey(ap_flame_state))
  {
    static unsigned long last_flip = millis();
    int value_fst = json[ap_flame_state].as<int>();
    const int flash_icon_pos = (outdoor_length == 0) ? 209 : (outdoor_length > 2) ? 150
                                                                                  : 162;
    if (fire_working_mode == 1)
    {
      tft_draw_bitmap(flash_icon_pos, 5, shower_burn, 16, 16);
    }
    else if (fire_working_mode == 2)
    {
      tft_draw_bitmap(flash_icon_pos, 5, radiator_burn, 16, 16);
    }
    else
    {
      tft_clear_rect(flash_icon_pos, 5, 16, 16, BLACK);
    }
    if (value_fst == 1 && current_mode == 1)
    {
      if (millis() - last_flip > 500)
      {
        last_flip = millis();
        fire_fliped = !fire_fliped;

        if (fire_fliped == false)
        {
          tft_draw_bitmap(155, 21, fire_modulate > 60 ? fire_large_rev : fire_modulate > 40 ? fire_normal_rev
                                                                                            : fire_small_rev,
                          48, 40);
        }
        else
        {
          tft_draw_bitmap(155, 21, fire_modulate > 60 ? fire_large : fire_modulate > 40 ? fire_normal
                                                                                        : fire_small,
                          48, 40);
        }
        tft_draw_round_rect(160, 60, 38, 5, 5, LIGHT_GRAY);
      }
    }
    else
    {
      tft_clear_rect(155, 21, 48, 40, BLACK);
      tft_draw_round_rect(160, 60, 38, 5, 5, LIGHT_GRAY);
    }
  }

  // draw the switch
  if (json.containsKey(ap_target_mode))
  {
    tft_draw_round_rect(209, 23, 16, 40, 5, target_mode ? LIGHT_GREEN : LIGHT_GRAY);
    if (target_mode)
    {
      tft_draw_round_rect(209, 23, 16, 22, 5, DARK_GREEN);
    }
    else
    {
      tft_draw_round_rect(209, 41, 16, 22, 5, DARK_GRAY);
    }
  }

  // draw operate hint
  if (millis() - wifi_rssi_last_ts > WIFI_SIGNAL_UPDATE_TIME)
  {
    currentOperateHint = (currentOperateHint + 1) % 2;

    wifi_rssi_last_ts = millis();
  }
  if (currentOperateHint == 0)
  {
#ifdef USE_CHINESE
    tft_draw_bitmap(0, 115, operate0, 240, 12);
#else
    tft_write(6, 118, 1, "SINGLE:SET,DOUBLE:ON/OFF,TRIPLE:SWITCH", ORANGE);
#endif
  }
  else
  {
#ifdef USE_CHINESE
    tft_draw_bitmap(0, 115, operate1, 240, 12);
#else
    tft_write(6, 118, 1, " FORTH:SHOW QRCODE,3SECS:AUTO/MANUAL  ", ORANGE);
#endif
  }

  if (millis() - last_mcast_ts > MCAST_TIMEOUT)
  {
    DebugPrintf("No multicast message received more than %d secs,", MCAST_TIMEOUT / 1000);
    DebugPrintln("Reconnect WiFi and seeking Airtub Partner again.");
    WiFi.reconnect();
    addr = IPAddress(0, 0, 0, 0);
    airtub_seeking = true;
    last_mcast_ts = millis();
  }

  if (addr == IPAddress(0, 0, 0, 0))
  {
    tft_draw_bitmap(60, 5, seeking, 16, 16);
    if (!initialized)
    {
      tft_clear_rect(0, 21, 240, 92, BLACK);
#ifdef USE_CHINESE
      tft_draw_bitmap(30, 55, ap_seeking, 140, 24);
#else
      tft_write(30, 55, 3, "Seeking...", YELLOW);
#endif
    }
  }
  else if (airtub_seeking)
  {
    airtub_seeking = false;
    if (!initialized)
    {
      tft_clear_rect(0, 21, 240, 92, BLACK);
      tft_update();
      initialized = true;
    }
  }

  // update all
  tft_update();

  // #ifdef DEBUG_ENABLED
  //   const unsigned long used_time = millis() - start_time;
  //   DebugPrintf("Used time for handling: %lu\n", used_time);
  // #endif

#ifdef USE_DEEP_SLEEP
  if (millis() - active_ts > DEEPSLEEP_TIME && power == LOW)
  {
    deep_sleep_now();
  }
#endif
  // DebugPrintf("Free Heap: %d\n", ESP.getFreeHeap());
}
