#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#undef TFT_RST
#undef TFT_RST
#undef TFT_MOSI
#undef TFT_SCK
#undef TFT_DC
#undef TFT_CS
#undef TFT_BL

#define TFT_RST -1 // we use the seesaw for resetting to save a pin
#define TFT_MOSI 33
#define TFT_SCK 34
#define TFT_DC 35
#define TFT_CS 36
#define TFT_BL 38
#define SPI_SPEED 40000000

#define LIGHT_GREEN 0x0F80
#define DARK_GREEN 0x0464
#define LIGHT_GRAY 0xBDF7
#define DARK_GRAY 0x7BCF
#define WHITE 0xFFFF
#define BLACK 0x0000
#define YELLOW 0xFFE0
#define RED 0xF800
#define BLUE 0x5D1C
#define ORANGE 0xF2A1

void tft_set_backlight(bool state);
void tft_init();
void tft_init(bool backlight);
void tft_clear(uint16_t color);
void tft_write_transperant(uint8_t x, uint8_t y, uint8_t size, const char *text, uint16_t color);
void tft_write(uint8_t x, uint8_t y, uint8_t size, const char *text, uint16_t color);
void tft_gen_qrcode(const char *text, const uint16_t color, const uint16_t background);
void tft_draw_bitmap(uint8_t x, uint8_t y, const uint16_t bitmap[], uint8_t w, uint8_t h);
void tft_draw_bitmap_reverse(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
void tft_clear_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void tft_draw_round_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);
void tft_update();
// void tft_set_font(const GFXfont *font);