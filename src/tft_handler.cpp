#include "tft_handler.h"
#include "qrcode.h"
#include <SPI.h>

float p = 3.1415926;

Adafruit_ST7789 *_tft = NULL;
QRCode qrcode;
GFXcanvas16 canvas(240, 135);

void tft_set_backlight(bool state)
{
    if (state)
    {
        digitalWrite(TFT_BL, HIGH);
    }
    else
    {
        digitalWrite(TFT_BL, LOW);
    }
}

void tft_init()
{
    SPIClass *spi = new SPIClass(HSPI);
    spi->begin(TFT_SCK, -1, TFT_MOSI, TFT_CS);
    _tft = new Adafruit_ST7789(spi, TFT_CS, TFT_DC, TFT_RST);
    _tft->init(135, 240);
    _tft->setSPISpeed(SPI_SPEED);
    uint16_t time = millis();
    _tft->fillScreen(ST77XX_BLACK);
    time = millis() - time;
    Serial.print("fillScreen spend:");
    Serial.print(time, DEC);
    Serial.println("ms");

    _tft->setRotation(3);
}

void tft_init(bool backlight)
{
    pinMode(TFT_BL, OUTPUT);
    tft_init();
    tft_set_backlight(backlight);
}

void tft_clear(uint16_t color)
{
    canvas.fillScreen(color);
}

void tft_write_transperant(uint8_t x, uint8_t y, uint8_t size, const char *text, uint16_t color)
{
    canvas.setCursor(x, y);
    canvas.setTextColor(color);
    canvas.setTextSize(size);
    canvas.print(text);
}

void tft_write(uint8_t x, uint8_t y, uint8_t size, const char *text, uint16_t color)
{
    canvas.setCursor(x, y);
    canvas.setTextColor(color, ST77XX_BLACK);
    canvas.setTextSize(size);
    canvas.print(text);
}

void tft_gen_qrcode(const char *text)
{
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    qrcode_initText(&qrcode, qrcodeData, 3, 0, text);
    canvas.fillScreen(ST77XX_WHITE);

    uint8_t scale = 4;
    uint16_t offsetX = (_tft->width() - qrcode.size * scale) / 2;
    uint16_t offsetY = (_tft->height() - qrcode.size * scale) / 2;

    for (uint8_t y = 0; y < qrcode.size; y++)
    {
        for (uint8_t x = 0; x < qrcode.size; x++)
        {
            if (qrcode_getModule(&qrcode, x, y))
            {
                // Add the offset to the x and y coordinates
                canvas.fillRect(x * scale + offsetX, y * scale + offsetY, scale, scale, ST77XX_BLACK);
            }
        }
    }
}

void tft_draw_bitmap(uint8_t x, uint8_t y, const uint16_t bitmap[], uint8_t w, uint8_t h)
{
    canvas.drawRGBBitmap(x, y, bitmap, w, h);
}

void tft_draw_bitmap_reverse(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h)
{
    for (int16_t j = 0; j < h; j++)
    {
        for (int16_t i = 0; i < w; i++)
        {
            uint16_t pixelColor = pgm_read_word(&bitmap[j * w + i]);
            canvas.drawPixel(x + w - i - 1, y + j, pixelColor);
        }
    }
}

void tft_clear_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
{
    canvas.fillRect(x, y, w, h, color);
}

void tft_draw_round_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color)
{
    canvas.fillRoundRect(x, y, w, h, r, color);
}

void tft_update()
{
    _tft->drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
}