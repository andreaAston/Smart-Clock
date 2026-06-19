#include "StopwatchManager.h"
#include "../utils/Helpers.h"

StopwatchManager::StopwatchManager()
    : _state(State::STOPPED), _accumulatedMs(0), _startMs(0) {}

void StopwatchManager::begin() {
    Serial.println(F("[STOPWATCH] Initialized"));
}

void StopwatchManager::toggleStartPause() {
    if (_state == State::RUNNING) {
        _accumulatedMs += millis() - _startMs;
        _state = State::PAUSED;
        Helpers::debugPrint("STOPWATCH", "Paused");
    } else {
        _startMs = millis();
        _state = State::RUNNING;
        Helpers::debugPrint("STOPWATCH", _accumulatedMs == 0 ? "Started" : "Resumed");
    }
}

void StopwatchManager::reset() {
    _state = State::STOPPED;
    _accumulatedMs = 0;
    Helpers::debugPrint("STOPWATCH", "Reset");
}

String StopwatchManager::getDisplayTime() const {
    unsigned long totalMs = _accumulatedMs;
    if (_state == State::RUNNING) {
        totalMs += millis() - _startMs;
    }
    unsigned long totalSeconds = totalMs / 1000;
    int h = totalSeconds / 3600;
    int m = (totalSeconds % 3600) / 60;
    int s = totalSeconds % 60;
    return Helpers::formatHMS(h, m, s);
}

String StopwatchManager::getStatusSuffix() const {
    switch (_state) {
        case State::RUNNING: return "RUN";
        case State::PAUSED:  return "PAU";
        default:             return "";
    }
}

bool StopwatchManager::isRunning() const {
    return _state == State::RUNNING;
}
