#include "RTCManager.h"

RTCManager::RTCManager(uint8_t datPin, uint8_t clkPin, uint8_t rstPin)
    : _wire(datPin, clkPin, rstPin), _rtc(_wire), _cached(0) {}

void RTCManager::begin() {
    _rtc.Begin();

    if (!_rtc.IsDateTimeValid()) {
        // RTC lost power (or is brand new) and has no valid stored time.
        // Fall back to a safe default so the system still boots cleanly.
        Serial.println(F("[RTC] Invalid time detected - setting default"));
        RtcDateTime defaultTime(2026, 1, 1, 0, 0, 0);
        _rtc.SetDateTime(defaultTime);
    }

    if (_rtc.GetIsWriteProtected()) {
        _rtc.SetIsWriteProtected(false);
    }
    if (!_rtc.GetIsRunning()) {
        _rtc.SetIsRunning(true);
    }

    _cached = _rtc.GetDateTime();
    Serial.println(F("[RTC] Initialized"));
}

void RTCManager::update() {
    _cached = _rtc.GetDateTime();
}

int RTCManager::getHour() const   { return _cached.Hour(); }
int RTCManager::getMinute() const { return _cached.Minute(); }
int RTCManager::getSecond() const { return _cached.Second(); }
int RTCManager::getDay() const    { return _cached.Day(); }
int RTCManager::getMonth() const  { return _cached.Month(); }
int RTCManager::getYear() const   { return _cached.Year(); }

void RTCManager::setDateTime(uint16_t year, uint8_t month, uint8_t day,
                              uint8_t hour, uint8_t minute, uint8_t second) {
    RtcDateTime dt(year, month, day, hour, minute, second);
    _rtc.SetDateTime(dt);
    _cached = dt;
    Serial.println(F("[RTC] Date/time updated"));
}

bool RTCManager::isRunning() const {
    return _rtc.GetIsRunning();
}
