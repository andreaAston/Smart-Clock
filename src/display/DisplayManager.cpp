#include "DisplayManager.h"
#include "../utils/Helpers.h"

DisplayManager::DisplayManager(uint8_t i2cAddress, uint8_t cols, uint8_t rows)
    : _lcd(i2cAddress, cols, rows), _lastLine1(""), _lastLine2(""), _firstDraw(true) {}

void DisplayManager::begin() {
    _lcd.begin(16, 2);
    _lcd.backlight();
    _lcd.clear();
    Serial.println(F("[DISPLAY] LCD initialized"));
}

void DisplayManager::writeLineIfChanged(uint8_t row, const String& content, String& cache) {
    String padded = Helpers::padRight(content, 16);
    if (padded == cache && !_firstDraw) {
        return; // nothing changed - skip the write to avoid flicker
    }
    _lcd.setCursor(0, row);
    _lcd.print(padded);
    cache = padded;
}

void DisplayManager::showLines(const String& line1, const String& line2) {
    writeLineIfChanged(0, line1, _lastLine1);
    writeLineIfChanged(1, line2, _lastLine2);
    _firstDraw = false;
}

void DisplayManager::showClock(const String& time, const String& temp,
                               const String& date, const String& humidity,
                               bool alarmOn) {
    String line1 = time + " " + temp;
    String line2 = date + " " + humidity + " " + (alarmOn ? "A" : " ");
    showLines(line1, line2);
}

void DisplayManager::showAlarmSet(const String& hhmm, bool alarmOn,
                                   bool editingHour, bool editingMinute) {
    String line1 = Helpers::centerText("ALARM");
    // Marker after the time shows the field currently being edited.
    String line2 = hhmm + " " + (alarmOn ? "ON " : "OFF");
    if (editingHour) line2 += " <H";
    if (editingMinute) line2 += " <M";
    showLines(line1, line2);
}

void DisplayManager::showCountdown(const String& hhmmss, const String& statusSuffix) {
    String line1 = Helpers::centerText("COUNTDOWN");
    String line2 = hhmmss + " " + statusSuffix;
    showLines(line1, line2);
}

void DisplayManager::showStopwatch(const String& hhmmss, const String& statusSuffix) {
    String line1 = Helpers::centerText("STOPWATCH");
    String line2 = hhmmss + " " + statusSuffix;
    showLines(line1, line2);
}

void DisplayManager::showAlarmTriggered() {
    String line1 = Helpers::centerText("ALARM!");
    String line2 = "PLAY=Stop NXT=Sz"; // exactly 16 chars, fits 16x2 LCD
    showLines(line1, line2);
}

void DisplayManager::showTimeUp() {
    String line1 = Helpers::centerText("TIME UP!");
    String line2 = Helpers::centerText("PREV to clear");
    showLines(line1, line2);
}
