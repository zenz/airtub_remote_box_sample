#include <Arduino.h>

struct udp_msg_format_t
{
    uint8_t type;
    uint8_t len;
    uint32_t crc;
    char data[180];
};
