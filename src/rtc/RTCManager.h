#pragma once
#include <Arduino.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

/**
 * RTCManager
 * ----------
 * Wraps the DS1302 RTC module (using the "Rtc by Makuna" library).
 * Provides simple getters for the current time/date. All other modules
 * read time exclusively through this class - nothing else touches the
 * RTC library directly.
 *
 * Call update() once per loop() to refresh the cached reading; the
 * getters below are then cheap and consistent for the rest of that loop.
 */
class RTCManager {
public:
    RTCManager(uint8_t datPin, uint8_t clkPin, uint8_t rstPin);
    void begin();
    void update();

    int getHour() const;
    int getMinute() const;
    int getSecond() const;
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    // For initial programming / debugging only.
    void setDateTime(uint16_t year, uint8_t month, uint8_t day,
                      uint8_t hour, uint8_t minute, uint8_t second);

    bool isRunning() const;

private:
    ThreeWire _wire;
    RtcDS1302<ThreeWire> _rtc;
    RtcDateTime _cached;
};
