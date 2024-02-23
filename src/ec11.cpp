#include "ec11.h"

OneButton button(PIN_BTN, true, false);
RotaryEncoder *encoder = nullptr;

// save the millis when a press has started.
unsigned long pressStartTime;
volatile int8_t buttonState = 0; // 按键状态

void ec11_init()
{
    encoder = new RotaryEncoder(PIN_CW, PIN_CCW, RotaryEncoder::LatchMode::TWO03);
    attachInterrupt(digitalPinToInterrupt(PIN_CW), check_ticks_rotary, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_CCW), check_ticks_rotary, CHANGE);

    attachInterrupt(digitalPinToInterrupt(PIN_BTN), check_ticks_button, CHANGE);
    button.attachClick(single_click);
    button.attachDoubleClick(double_click);
    button.attachMultiClick(multi_click);

    button.setPressMs(LONG_PRESS_MS);
    button.attachLongPressStart(press_start);
    button.attachLongPressStop(press_stop);
}

void IRAM_ATTR check_ticks_button()
{
    // include all buttons here to be checked
    button.tick(); // just call tick() to check the state.
}

void IRAM_ATTR check_ticks_rotary()
{
    encoder->tick();
}

// this function will be called when the button was pressed 1 time only.
void single_click()
{
    buttonState = 1;
} // singleClick

// this function will be called when the button was pressed 2 times in a short timeframe.
void double_click()
{
    buttonState = 2;
} // doubleClick

// this function will be called when the button was pressed multiple times in a short timeframe.
void multi_click()
{
    buttonState = button.getNumberClicks();
} // multiClick

// this function will be called when the button was held down for 1 second or more.
void press_start()
{
    pressStartTime = millis() - START_TICKS; // as set in setPressTicks()
} // pressStart()

// this function will be called when the button was released after a long hold.
void press_stop()
{
    const int sec = (millis() - pressStartTime) * START_TICKS_RECIPROCAL + 1;
    buttonState = -sec;
} // pressStop()