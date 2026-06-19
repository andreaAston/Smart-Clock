#include "BuzzerManager.h"

BuzzerManager::BuzzerManager(uint8_t pin)
    : _pin(pin), _beeping(false), _beepEndMs(0),
      _patternActive(false), _patternState(false),
      _patternOnMs(0), _patternOffMs(0), _patternLastToggleMs(0) {}

void BuzzerManager::begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    Serial.println(F("[BUZZER] Initialized"));
}

void BuzzerManager::on() {
    digitalWrite(_pin, HIGH);
}

void BuzzerManager::off() {
    digitalWrite(_pin, LOW);
    _beeping = false;
    _patternActive = false;
}

void BuzzerManager::beep(unsigned long durationMs) {
    digitalWrite(_pin, HIGH);
    _beeping = true;
    _beepEndMs = millis() + durationMs;
}

void BuzzerManager::startPattern(unsigned long onMs, unsigned long offMs) {
    _patternActive = true;
    _patternOnMs = onMs;
    _patternOffMs = offMs;
    _patternState = true;
    _patternLastToggleMs = millis();
    digitalWrite(_pin, HIGH);
    Serial.println(F("[BUZZER] Pattern started"));
}

void BuzzerManager::stopPattern() {
    _patternActive = false;
    digitalWrite(_pin, LOW);
    Serial.println(F("[BUZZER] Pattern stopped"));
}

void BuzzerManager::update() {
    // A one-shot beep takes priority over any repeating pattern.
    if (_beeping) {
        if (millis() >= _beepEndMs) {
            digitalWrite(_pin, LOW);
            _beeping = false;
        }
        return;
    }

    if (_patternActive) {
        unsigned long now = millis();
        unsigned long interval = _patternState ? _patternOnMs : _patternOffMs;
        if (now - _patternLastToggleMs >= interval) {
            _patternLastToggleMs = now;
            _patternState = !_patternState;
            digitalWrite(_pin, _patternState ? HIGH : LOW);
        }
    }
}
