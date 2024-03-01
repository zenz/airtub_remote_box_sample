#ifndef PARTNER_BUTTON_H_
#define PARTNER_BUTTON_H_

#include <Arduino.h>
#include <OneButton.h>
#include <RotaryEncoder.h>

#define PIN_BTN 18
#define LONG_PRESS_MS 2000
#define START_TICKS 1000
#define START_TICKS_RECIPROCAL 0.001

#define PIN_CW 41
#define PIN_CCW 42

extern OneButton button;
extern RotaryEncoder *encoder;
extern volatile int8_t buttonState;

void ec11_init();
void IRAM_ATTR check_ticks_button();
void IRAM_ATTR check_ticks_rotary();
void single_click();
void double_click();
void multi_click();
void press_start();
void press_stop();

#endif // PARTNER_BUTTON_H_