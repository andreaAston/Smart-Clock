#pragma once
#include <Arduino.h>
#include "../hardware/BuzzerManager.h"

/**
 * CountdownTimer
 * --------------
 * Implements the countdown timer screen as a small state machine:
 * IDLE (entering digits) -> RUNNING <-> PAUSED -> FINISHED.
 *
 * Digits are entered like a kitchen timer: each new digit shifts the
 * previous ones left, e.g. typing 5, 0, 0 while idle builds "00:05:00".
 */
class CountdownTimer {
public:
    explicit CountdownTimer(BuzzerManager& buzzer);
    void begin();

    void addDigit(int digit);
    void clearDigits();
    void toggleStartPause();
    void stopAndReset();
    void update(); // call every loop(); ticks down once per second

    String getDisplayTime() const;
    String getStatusSuffix() const;
    bool isFinished() const;
    bool isIdleAndEmpty() const; // true if nothing entered/started yet

private:
    enum class State { IDLE, RUNNING, PAUSED, FINISHED };
    State _state;
    BuzzerManager& _buzzer;

    char _digits[6]; // H H M M S S, each a char '0'-'9'
    long _remainingSeconds;
    unsigned long _lastTickMs;

    long digitsToSeconds() const;
};
