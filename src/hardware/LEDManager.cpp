#include "LEDManager.h"

LEDManager::LEDManager(uint8_t pin)
    : _pin(pin), _blinking(false), _ledState(false),
      _intervalMs(500), _lastToggleMs(0) {}

void LEDManager::begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    Serial.println(F("[LED] Initialized"));
}

void LEDManager::on() {
    _blinking = false;
    _ledState = true;
    digitalWrite(_pin, HIGH);
}

void LEDManager::off() {
    _blinking = false;
    _ledState = false;
    digitalWrite(_pin, LOW);
}

void LEDManager::startBlinking(unsigned long intervalMs) {
    _blinking = true;
    _intervalMs = intervalMs;
    _lastToggleMs = millis();
    Serial.println(F("[LED] Blinking started"));
}

void LEDManager::stopBlinking() {
    _blinking = false;
    digitalWrite(_pin, LOW);
    Serial.println(F("[LED] Blinking stopped"));
}

void LEDManager::update() {
    if (!_blinking) return;
    unsigned long now = millis();
    if (now - _lastToggleMs >= _intervalMs) {
        _lastToggleMs = now;
        _ledState = !_ledState;
        digitalWrite(_pin, _ledState ? HIGH : LOW);
    }
}
