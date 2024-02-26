#include <Arduino.h>

const unsigned short fire_rev[2352] PROGMEM = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0010 (16) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0020 (32) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0030 (48) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0040 (64) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0050 (80) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0060 (96) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0070 (112) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0080 (128) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0090 (144) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x00A0 (160) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFA40, 0xFAA0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x00B0 (176) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x00C0 (192) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x00D0 (208) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x00E0 (224) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x00F0 (240) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0100 (256) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0110 (272) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0120 (288) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA80, 0xFA20, 0xF800, 0x0000, // 0x0130 (304) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFA20, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0140 (320) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0150 (336) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFA60, 0xFA20, 0x0000, // 0x0160 (352) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0xF800, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0170 (368) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0180 (384) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFA60, 0xFA40, 0x0000, // 0x0190 (400) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0xFA00, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x01A0 (416) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x01B0 (432) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFA60, 0xFA40, 0x0000, // 0x01C0 (448) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0xFA00, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x01D0 (464) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x01E0 (480) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFA60, 0xFA40, 0x0000, // 0x01F0 (496) pixels
    0x0000, 0x0000, 0xF800, 0xFA80, 0xFA60, 0xFB20, 0xFB20, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0200 (512) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0210 (528) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFA60, 0xFA40, 0x0000, // 0x0220 (544) pixels
    0x0000, 0x0000, 0xFA40, 0xFA40, 0xFA80, 0xFE60, 0xFF60, 0xFAE0, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0230 (560) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0240 (576) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFA60, 0xFA40, 0x0000, // 0x0250 (592) pixels
    0x0000, 0xFA40, 0xFA80, 0xFBE0, 0xFCA0, 0xFF20, 0xFFE0, 0xFAE0, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0260 (608) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0270 (624) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFA60, 0xFA40, 0x0000, // 0x0280 (640) pixels
    0x0000, 0xFA40, 0xFA80, 0xFD20, 0xFF80, 0xFFE0, 0xFFE0, 0xFAE0, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0290 (656) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x02A0 (672) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFA40, 0xFA60, 0xFA60, // 0x02B0 (688) pixels
    0xFA40, 0xFC00, 0xFD20, 0xFE80, 0xFFC0, 0xFDC0, 0xFD20, 0xFAC0, 0xFA40, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x02C0 (704) pixels
    0x0000, 0xFA60, 0xFA60, 0xFA60, 0xFA00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x02D0 (720) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, // 0x02E0 (736) pixels
    0xFA60, 0xFCA0, 0xFFE0, 0xFFE0, 0xFFA0, 0xFBA0, 0xFA60, 0xFA00, 0x0000, 0x0000, 0xFA00, 0xFAA0, 0xFA40, 0x0000, 0x0000, 0x0000, // 0x02F0 (752) pixels
    0x0000, 0xFA60, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0300 (768) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, // 0x0310 (784) pixels
    0xFA60, 0xFCA0, 0xFFE0, 0xFFE0, 0xFFA0, 0xFBA0, 0xFA60, 0xFA00, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, // 0x0320 (800) pixels
    0x0000, 0xFA60, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0330 (816) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, // 0x0340 (832) pixels
    0xFA60, 0xFCA0, 0xFFE0, 0xFFE0, 0xFFA0, 0xFBA0, 0xFA60, 0xFA00, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, // 0x0350 (848) pixels
    0x0000, 0xFA60, 0xFA60, 0xFA60, 0xFA40, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0360 (864) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, // 0x0370 (880) pixels
    0xFA60, 0xFCA0, 0xFFE0, 0xFFE0, 0xFFA0, 0xFBA0, 0xFA60, 0xFA00, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, // 0x0380 (896) pixels
    0x0000, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0390 (912) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, // 0x03A0 (928) pixels
    0xFA60, 0xFCA0, 0xFFE0, 0xFFE0, 0xFFA0, 0xFBA0, 0xFA60, 0xFA00, 0xF800, 0xF800, 0xFA40, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, // 0x03B0 (944) pixels
    0x0000, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x03C0 (960) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, // 0x03D0 (976) pixels
    0xFA60, 0xFCA0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFA0, 0xFFA0, 0xFAE0, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, // 0x03E0 (992) pixels
    0x0000, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x03F0 (1008) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, // 0x0400 (1024) pixels
    0xFA60, 0xFC40, 0xFE40, 0xFF00, 0xFFE0, 0xFFE0, 0xFFE0, 0xFC60, 0xFC00, 0xFC00, 0xFB60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, // 0x0410 (1040) pixels
    0x0000, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0420 (1056) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFAA0, // 0x0430 (1072) pixels
    0xFA20, 0xFAC0, 0xFB00, 0xFD80, 0xFFE0, 0xFFE0, 0xFFE0, 0xFF60, 0xFF60, 0xFF60, 0xFD80, 0xFA80, 0xFA40, 0x0000, 0x0000, 0x0000, // 0x0440 (1088) pixels
    0x0000, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0450 (1104) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, // 0x0460 (1120) pixels
    0xFA40, 0xFA40, 0xFA80, 0xFC60, 0xFD60, 0xFF40, 0xFFE0, 0xFFE0, 0xFF40, 0xFDA0, 0xFCA0, 0xFA80, 0xFA40, 0x0000, 0x0000, 0x0000, // 0x0470 (1136) pixels
    0x0000, 0xFA60, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0480 (1152) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, // 0x0490 (1168) pixels
    0xFA60, 0xFA60, 0xF9C0, 0xFA60, 0xFA80, 0xFEA0, 0xFFE0, 0xFFE0, 0xFEA0, 0xFB20, 0xFA40, 0xFA40, 0x0000, 0x0000, 0xF800, 0xFAA0, // 0x04A0 (1184) pixels
    0x0000, 0xFA60, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x04B0 (1200) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, // 0x04C0 (1216) pixels
    0xFA60, 0xFA60, 0xF9C0, 0xFA60, 0xFA80, 0xFEA0, 0xFFE0, 0xFFE0, 0xFEA0, 0xFB20, 0xFA40, 0xFA40, 0x0000, 0xFAA0, 0xFA40, 0xFA40, // 0x04D0 (1232) pixels
    0xFA20, 0xFA60, 0xFA60, 0xFA60, 0xFA00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x04E0 (1248) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xF800, 0xFA60, 0xFA20, 0x0000, 0x0000, 0x0000, 0xFA40, // 0x04F0 (1264) pixels
    0xFA60, 0xFA60, 0xFA80, 0xFAC0, 0xFAE0, 0xFEC0, 0xFFE0, 0xFFE0, 0xFEA0, 0xFB20, 0xFA40, 0xFA40, 0x0000, 0xFA00, 0xFA60, 0xFA60, // 0x0500 (1280) pixels
    0xFA00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0510 (1296) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFA60, 0xFA40, 0xF980, 0x0000, 0x0000, 0xFA40, // 0x0520 (1312) pixels
    0xFA60, 0xFA60, 0xFA80, 0xFCC0, 0xFE60, 0xFF80, 0xFFE0, 0xFFE0, 0xFEA0, 0xFB20, 0xFA40, 0xFA40, 0x0000, 0xFA00, 0xFA60, 0xFA60, // 0x0530 (1328) pixels
    0xFA00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0540 (1344) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0xFA40, // 0x0550 (1360) pixels
    0xFA60, 0xFA60, 0xFA80, 0xFD20, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFEA0, 0xFB20, 0xFA40, 0xFA40, 0x0000, 0xFA00, 0xFA60, 0xFA60, // 0x0560 (1376) pixels
    0xFA00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0570 (1392) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0xFA40, // 0x0580 (1408) pixels
    0xFA60, 0xFCA0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFEA0, 0xFB20, 0xFA40, 0xFA40, 0x0000, 0xFA00, 0xFA60, 0xFA60, // 0x0590 (1424) pixels
    0xFA00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x05A0 (1440) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0xFA40, // 0x05B0 (1456) pixels
    0xFA60, 0xFCA0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFEE0, 0xFC20, 0xFB60, 0xFA80, 0xFA40, 0xFA00, 0xFA60, 0xFA60, // 0x05C0 (1472) pixels
    0xFA40, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x05D0 (1488) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0xFA40, // 0x05E0 (1504) pixels
    0xFA60, 0xFCA0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFC0, 0xFF60, 0xFDA0, 0xFA80, 0xFA60, 0xFA00, 0xFA60, 0xFA60, // 0x05F0 (1520) pixels
    0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0600 (1536) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0xFA40, // 0x0610 (1552) pixels
    0xFA60, 0xFC20, 0xFDA0, 0xFEC0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFDC0, 0xFA80, 0xFA40, 0xFA00, 0xFA60, 0xFA60, // 0x0620 (1568) pixels
    0xFC60, 0xFCA0, 0xFB20, 0xFA40, 0xFA20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0630 (1584) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA60, 0xFA40, 0x0000, 0x0000, 0xFAA0, // 0x0640 (1600) pixels
    0xFAA0, 0xFA80, 0xFAC0, 0xFD60, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFDC0, 0xFA80, 0xFA40, 0xFA00, 0xFA60, 0xFA60, // 0x0650 (1616) pixels
    0xFEE0, 0xFF20, 0xFB40, 0xFA60, 0xFA60, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0660 (1632) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFAA0, 0xFC80, 0xFD00, 0xFA60, 0xFA40, 0xFAA0, 0x0000, // 0x0670 (1648) pixels
    0x0000, 0xFA40, 0xFAA0, 0xFD40, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFEC0, 0xFD20, 0xFC60, 0xFA80, 0xFC60, 0xFD00, // 0x0680 (1664) pixels
    0xFF80, 0xFF60, 0xFB40, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0690 (1680) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFAC0, 0xFE00, 0xFFE0, 0xFAC0, 0xFAC0, 0xFA60, 0xFA00, // 0x06A0 (1696) pixels
    0xFA00, 0xFAA0, 0xFAE0, 0xFD60, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFDC0, 0xFAE0, 0xFDC0, 0xFFE0, // 0x06B0 (1712) pixels
    0xFFE0, 0xFF60, 0xFB80, 0xFAC0, 0xFAA0, 0xFAA0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x06C0 (1728) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFAC0, 0xFE00, 0xFFE0, 0xFDE0, 0xFD80, 0xFAE0, 0xFA60, // 0x06D0 (1744) pixels
    0xFA60, 0xFC20, 0xFDE0, 0xFEE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFF00, 0xFDE0, 0xFF00, 0xFFE0, // 0x06E0 (1760) pixels
    0xFFE0, 0xFFC0, 0xFE20, 0xFD20, 0xFB00, 0xFA60, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x06F0 (1776) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFAC0, 0xFE00, 0xFFE0, 0xFFE0, 0xFF20, 0xFB20, 0xFA80, // 0x0700 (1792) pixels
    0xFA80, 0xFCC0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, // 0x0710 (1808) pixels
    0xFFE0, 0xFFE0, 0xFFE0, 0xFE40, 0xFB00, 0xFA60, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0720 (1824) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFAC0, 0xFE00, 0xFFE0, 0xFFE0, 0xFFE0, 0xFF80, 0xFF60, // 0x0730 (1840) pixels
    0xFF60, 0xFFA0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, // 0x0740 (1856) pixels
    0xFFE0, 0xFFE0, 0xFFE0, 0xFE40, 0xFB00, 0xFA60, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0750 (1872) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFAC0, 0xFE00, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, // 0x0760 (1888) pixels
    0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, // 0x0770 (1904) pixels
    0xFFE0, 0xFFE0, 0xFFE0, 0xFE40, 0xFB00, 0xFA60, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0780 (1920) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA60, 0xFAC0, 0xFE00, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, // 0x0790 (1936) pixels
    0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, // 0x07A0 (1952) pixels
    0xFFE0, 0xFFE0, 0xFFE0, 0xFE40, 0xFB00, 0xFA60, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x07B0 (1968) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFAA0, 0xFD00, 0xFDE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, // 0x07C0 (1984) pixels
    0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, // 0x07D0 (2000) pixels
    0xFFE0, 0xFFC0, 0xFE40, 0xFD20, 0xFB00, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x07E0 (2016) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA00, 0xFA60, 0xFAC0, 0xFAC0, 0xFFC0, 0xFFE0, 0xFFE0, 0xFFE0, // 0x07F0 (2032) pixels
    0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, // 0x0800 (2048) pixels
    0xFFE0, 0xFF60, 0xFBA0, 0xFAC0, 0xFA80, 0xFA20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0810 (2064) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFAA0, 0xFA60, 0xFA40, 0xFD40, 0xFDA0, 0xFFA0, 0xFFE0, // 0x0820 (2080) pixels
    0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, // 0x0830 (2096) pixels
    0xFD80, 0xFD20, 0xFB20, 0xFA40, 0xFA60, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0840 (2112) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFB20, 0xFF00, 0xFFA0, // 0x0850 (2128) pixels
    0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, 0xFFA0, // 0x0860 (2144) pixels
    0xFB00, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0870 (2160) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFB00, 0xFCA0, 0xFCE0, // 0x0880 (2176) pixels
    0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, 0xFCE0, // 0x0890 (2192) pixels
    0xFAE0, 0xFA60, 0xFA40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x08A0 (2208) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFA60, 0xFA60, // 0x08B0 (2224) pixels
    0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, // 0x08C0 (2240) pixels
    0xFA00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x08D0 (2256) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFA40, 0xFA40, 0xFA60, // 0x08E0 (2272) pixels
    0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, 0xFA60, // 0x08F0 (2288) pixels
    0xFA00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0900 (2304) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0910 (2320) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0920 (2336) pixels
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // 0x0930 (2352) pixels
};