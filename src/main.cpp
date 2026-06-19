/**
 * Multi-function Clock / Alarm / Countdown / Stopwatch
 * ------------------------------------------------------
 * Hardware: Arduino Uno, DS1302 RTC, DHT11, 16x2 I2C LCD (0x27),
 *           IR receiver (NEC remote), active buzzer, status LED.
 *
 * This file only wires hardware pins to each manager class and runs
 * the main loop. All actual logic lives in the dedicated modules
 * under src/ - see MenuManager for the central state machine.
 */
#include <Arduino.h>
#include "ir/IRRemoteManager.h"
#include "rtc/RTCManager.h"
#include "sensors/SensorManager.h"
#include "display/DisplayManager.h"
#include "hardware/BuzzerManager.h"
#include "hardware/LEDManager.h"
#include "alarm/AlarmManager.h"
#include "countdown/CountdownTimer.h"
#include "stopwatch/StopwatchManager.h"
#include "menu/MenuManager.h"

// ----- Pin assignments (per project wiring table) -----
const uint8_t PIN_IR_RECEIVER = 3;
const uint8_t PIN_BUZZER      = 4;
const uint8_t PIN_LED_GREEN   = 13;
const uint8_t PIN_DHT11       = 2;
const uint8_t PIN_RTC_DAT     = 7;
const uint8_t PIN_RTC_CLK     = 6;
const uint8_t PIN_RTC_RST     = 8;
const uint8_t LCD_I2C_ADDRESS = 0x27;

// ----- Module instances. Each owns its own state; no loose globals. -----
IRRemoteManager irRemote(PIN_IR_RECEIVER);
RTCManager rtc(PIN_RTC_DAT, PIN_RTC_CLK, PIN_RTC_RST);
SensorManager sensors(PIN_DHT11);
DisplayManager display(LCD_I2C_ADDRESS, 16, 2);
BuzzerManager buzzer(PIN_BUZZER);
LEDManager led(PIN_LED_GREEN);
AlarmManager alarm(buzzer, led);
CountdownTimer countdown(buzzer);
StopwatchManager stopwatch;
MenuManager menu(rtc, sensors, display, alarm, countdown, stopwatch);

void setup() {
    Serial.begin(9600);
    Serial.println(F("=== System booting ==="));

    irRemote.begin();
    rtc.begin();
    sensors.begin();
    display.begin();
    buzzer.begin();
    led.begin();
    alarm.begin();
    countdown.begin();
    stopwatch.begin();
    menu.begin();

    Serial.println(F("=== System ready ==="));
}

void loop() {
    IRButton button = irRemote.poll();
    if (button != IRButton::NONE) {
        // Audible feedback for any remote button press
        buzzer.beep(120); // short 120ms confirmation beep

        menu.handleInput(button);
    }

    menu.update();
    buzzer.update();
    led.update();
}
