#pragma once
#include <Arduino.h>

/**
 * LEDManager
 * ----------
 * Controls the green status LED (pin 13, with inline 220ohm resistor).
 * Supports steady on/off plus a non-blocking blink mode, used to
 * indicate "alarm ringing" state without ever calling delay().
 */
class LEDManager {
public:
    explicit LEDManager(uint8_t pin);
    void begin();
    void on();
    void off();

    // Enables blinking at the given interval (ms). Call update() every loop.
    void startBlinking(unsigned long intervalMs);
    void stopBlinking();
    void update();

private:
    uint8_t _pin;
    bool _blinking;
    bool _ledState;
    unsigned long _intervalMs;
    unsigned long _lastToggleMs;
};
