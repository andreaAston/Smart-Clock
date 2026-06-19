#pragma once
#include <Arduino.h>

/**
 * BuzzerManager
 * -------------
 * Controls the active buzzer (pin 4). Provides a one-shot timed beep
 * and a non-blocking repeating on/off pattern (used for alarm ringing
 * and "time up" notification). update() must be called every loop()
 * to service any in-progress beep/pattern without using delay().
 */
class BuzzerManager {
public:
    explicit BuzzerManager(uint8_t pin);
    void begin();
    void on();
    void off();

    // Starts a single beep that auto-stops after durationMs.
    void beep(unsigned long durationMs);

    // Starts a repeating on/off pattern (e.g. startPattern(300, 300)).
    void startPattern(unsigned long onMs, unsigned long offMs);
    void stopPattern();

    void update();

private:
    uint8_t _pin;
    bool _beeping;
    unsigned long _beepEndMs;

    bool _patternActive;
    bool _patternState;
    unsigned long _patternOnMs;
    unsigned long _patternOffMs;
    unsigned long _patternLastToggleMs;
};
