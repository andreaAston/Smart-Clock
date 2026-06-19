#include "CountdownTimer.h"
#include "../utils/Helpers.h"

CountdownTimer::CountdownTimer(BuzzerManager& buzzer)
    : _state(State::IDLE), _buzzer(buzzer), _remainingSeconds(0), _lastTickMs(0) {
    for (int i = 0; i < 6; i++) _digits[i] = '0';
}

void CountdownTimer::begin() {
    Serial.println(F("[COUNTDOWN] Initialized"));
}

void CountdownTimer::addDigit(int digit) {
    if (_state != State::IDLE) {
        Helpers::debugPrint("COUNTDOWN", "Ignored digit - timer not idle");
        return;
    }
    // Shift all digits left, insert the new one at the rightmost (seconds) slot.
    for (int i = 0; i < 5; i++) {
        _digits[i] = _digits[i + 1];
    }
    _digits[5] = '0' + digit;
    Helpers::debugPrint("COUNTDOWN", "Digit entered");
}

void CountdownTimer::clearDigits() {
    if (_state != State::IDLE) return;
    for (int i = 0; i < 6; i++) _digits[i] = '0';
    Helpers::debugPrint("COUNTDOWN", "Digits cleared");
}

void CountdownTimer::toggleStartPause() {
    if (_state == State::IDLE) {
        _remainingSeconds = digitsToSeconds();
        if (_remainingSeconds <= 0) {
            Helpers::debugPrint("COUNTDOWN", "Cannot start - time is zero");
            return;
        }
        _state = State::RUNNING;
        _lastTickMs = millis();
        Helpers::debugPrint("COUNTDOWN", "Started");
    } else if (_state == State::RUNNING) {
        _state = State::PAUSED;
        Helpers::debugPrint("COUNTDOWN", "Paused");
    } else if (_state == State::PAUSED) {
        _state = State::RUNNING;
        _lastTickMs = millis();
        Helpers::debugPrint("COUNTDOWN", "Resumed");
    } else if (_state == State::FINISHED) {
        stopAndReset();
    }
}

void CountdownTimer::stopAndReset() {
    _state = State::IDLE;
    _remainingSeconds = 0;
    for (int i = 0; i < 6; i++) _digits[i] = '0';
    _buzzer.stopPattern();
    Helpers::debugPrint("COUNTDOWN", "Stopped and reset");
}

void CountdownTimer::update() {
    if (_state != State::RUNNING) return;
    unsigned long now = millis();
    if (now - _lastTickMs >= 1000) {
        _lastTickMs += 1000;
        _remainingSeconds--;
        if (_remainingSeconds <= 0) {
            _remainingSeconds = 0;
            _state = State::FINISHED;
            _buzzer.startPattern(500, 500);
            Helpers::debugPrint("COUNTDOWN", "Time up!");
        }
    }
}

String CountdownTimer::getDisplayTime() const {
    if (_state == State::IDLE) {
        int h = (_digits[0] - '0') * 10 + (_digits[1] - '0');
        int m = (_digits[2] - '0') * 10 + (_digits[3] - '0');
        int s = (_digits[4] - '0') * 10 + (_digits[5] - '0');
        return Helpers::formatHMS(h, m, s);
    }
    long secs = _remainingSeconds;
    int h = secs / 3600;
    int m = (secs % 3600) / 60;
    int s = secs % 60;
    return Helpers::formatHMS(h, m, s);
}

String CountdownTimer::getStatusSuffix() const {
    switch (_state) {
        case State::RUNNING:  return "RUN";
        case State::PAUSED:   return "PAU";
        case State::FINISHED: return "FIN";
        default:              return "";
    }
}

bool CountdownTimer::isFinished() const {
    return _state == State::FINISHED;
}

bool CountdownTimer::isIdleAndEmpty() const {
    if (_state != State::IDLE) return false;
    for (int i = 0; i < 6; i++) {
        if (_digits[i] != '0') return false;
    }
    return true;
}

long CountdownTimer::digitsToSeconds() const {
    int h = (_digits[0] - '0') * 10 + (_digits[1] - '0');
    int m = (_digits[2] - '0') * 10 + (_digits[3] - '0');
    int s = (_digits[4] - '0') * 10 + (_digits[5] - '0');
    m = Helpers::clampInt(m, 0, 59);
    s = Helpers::clampInt(s, 0, 59);
    return (long)h * 3600 + (long)m * 60 + s;
}
