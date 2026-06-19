#include "AlarmManager.h"
#include "../utils/Helpers.h"

AlarmManager::AlarmManager(BuzzerManager& buzzer, LEDManager& led)
    : _buzzer(buzzer), _led(led), _hour(7), _minute(30), _enabled(false),
      _editField(EditField::NONE), _editHour(7), _editMinute(30), _editBuffer(""),
      _triggered(false), _snoozing(false), _snoozeCount(0), _snoozeUntilMs(0) {}

void AlarmManager::begin() {
    Serial.println(F("[ALARM] Initialized"));
}

void AlarmManager::enterSetMode() {
    _editHour = _hour;
    _editMinute = _minute;
    _editField = EditField::HOUR;
    _editBuffer = "";
    Helpers::debugPrint("ALARM", "Entered set mode");
}

void AlarmManager::handleDigit(int digit) {
    if (_editField == EditField::NONE) return;

    // Two digits per field; a third digit press starts a fresh entry.
    if (_editBuffer.length() >= 2) {
        _editBuffer = "";
    }
    _editBuffer += String(digit);
    int value = _editBuffer.toInt();

    if (_editField == EditField::HOUR) {
        _editHour = Helpers::clampInt(value, 0, 23);
    } else {
        _editMinute = Helpers::clampInt(value, 0, 59);
    }
    Helpers::debugPrint("ALARM", "Digit buffer updated: " + _editBuffer);
}

void AlarmManager::nextField() {
    _editBuffer = "";
    _editField = (_editField == EditField::HOUR) ? EditField::MINUTE : EditField::HOUR;
    Helpers::debugPrint("ALARM", _editField == EditField::HOUR ? "Editing hour" : "Editing minute");
}

void AlarmManager::confirmAndSave() {
    _hour = _editHour;
    _minute = _editMinute;
    _editField = EditField::NONE;
    Helpers::debugPrint("ALARM", "Alarm set to " + Helpers::formatHM(_hour, _minute));
}

void AlarmManager::cancelEdit() {
    _editField = EditField::NONE;
    Helpers::debugPrint("ALARM", "Edit cancelled");
}

void AlarmManager::toggleEnabled() {
    _enabled = !_enabled;
    Helpers::debugPrint("ALARM", _enabled ? "Enabled" : "Disabled");
}

void AlarmManager::checkTrigger(int currentHour, int currentMinute, int currentSecond) {
    // Re-fire after a snooze period elapses.
    if (_snoozing) {
        if (millis() >= _snoozeUntilMs) {
            _snoozing = false;
            _triggered = true;
            _buzzer.startPattern(300, 300);
            _led.startBlinking(300);
            Helpers::debugPrint("ALARM", "Snooze ended - ringing again");
        }
        return;
    }

    if (!_enabled || _triggered) return;

    if (currentHour == _hour && currentMinute == _minute && currentSecond == 0) {
        _triggered = true;
        _buzzer.startPattern(300, 300);
        _led.startBlinking(300);
        Helpers::debugPrint("ALARM", "Alarm triggered!");
    }
}

bool AlarmManager::isTriggered() const {
    return _triggered;
}

void AlarmManager::stopAlarm() {
    _triggered = false;
    _snoozing = false;
    _snoozeCount = 0;
    _buzzer.stopPattern();
    _led.stopBlinking();
    Helpers::debugPrint("ALARM", "Alarm stopped");
}

void AlarmManager::snooze() {
    if (_snoozeCount >= MAX_SNOOZES) {
        Helpers::debugPrint("ALARM", "Max snoozes reached - stopping");
        stopAlarm();
        return;
    }
    _snoozeCount++;
    _snoozing = true;
    _triggered = false;
    _snoozeUntilMs = millis() + SNOOZE_DURATION_MS;
    _buzzer.stopPattern();
    _led.stopBlinking();
    Helpers::debugPrint("ALARM", "Snoozed, count=" + String(_snoozeCount));
}

bool AlarmManager::isEnabled() const { return _enabled; }
bool AlarmManager::isEditingHour() const { return _editField == EditField::HOUR; }
bool AlarmManager::isEditingMinute() const { return _editField == EditField::MINUTE; }

String AlarmManager::getDisplayHHMM() const {
    if (_editField != EditField::NONE) {
        return Helpers::formatHM(_editHour, _editMinute);
    }
    return Helpers::formatHM(_hour, _minute);
}
