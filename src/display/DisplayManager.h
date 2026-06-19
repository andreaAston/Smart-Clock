#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

/**
 * DisplayManager
 * --------------
 * Wraps the 16x2 I2C LCD (address 0x27). Every screen in the app funnels
 * through this class's showXxx() methods.
 *
 * To avoid flicker, it caches the last string written to each row and
 * only calls lcd.print() again when the row's content actually changed
 * (writeLineIfChanged). This satisfies "update only changed fields".
 */
class DisplayManager {
public:
    DisplayManager(uint8_t i2cAddress, uint8_t cols, uint8_t rows);
    void begin();

    // Generic two-line write. Internally diffs against the cache.
    void showLines(const String& line1, const String& line2);

    // --- Screen builders, one per app mode ---
    void showClock(const String& time, const String& temp,
                   const String& date, const String& humidity,
                   bool alarmOn);
    void showAlarmSet(const String& hhmm, bool alarmOn,
                       bool editingHour, bool editingMinute);
    void showCountdown(const String& hhmmss, const String& statusSuffix);
    void showStopwatch(const String& hhmmss, const String& statusSuffix);
    void showAlarmTriggered();
    void showTimeUp();

private:
    LiquidCrystal_I2C _lcd;
    String _lastLine1;
    String _lastLine2;
    bool _firstDraw;

    void writeLineIfChanged(uint8_t row, const String& content, String& cache);
};
