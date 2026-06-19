#pragma once
#include <Arduino.h>

/**
 * StopwatchManager
 * ----------------
 * Implements the stopwatch screen: a Start/Pause/Reset state machine
 * driven entirely by millis() deltas - no blocking delay() calls.
 */
class StopwatchManager {
public:
    StopwatchManager();
    void begin();

    void toggleStartPause();
    void reset();

    String getDisplayTime() const;
    String getStatusSuffix() const;
    bool isRunning() const;

private:
    enum class State { STOPPED, RUNNING, PAUSED };
    State _state;
    unsigned long _accumulatedMs;
    unsigned long _startMs;
};
