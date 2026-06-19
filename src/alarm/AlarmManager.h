#pragma once
#include <Arduino.h>
#include "../hardware/BuzzerManager.h"
#include "../hardware/LEDManager.h"

/**
 * AlarmManager
 * ------------
 * Implements the Alarm feature: time-of-day setting (with Hour/Minute
 * field editing), an enable/disable toggle, and the ringing/snooze
 * behavior once the alarm matches the current RTC time.
 *
 * This class owns the Buzzer and LED while ringing - MenuManager only
 * tells it when to check/stop/snooze, it never drives the hardware itself.
 */
class AlarmManager {
public:
    AlarmManager(BuzzerManager& buzzer, LEDManager& led);
    void begin();

    // --- Setting screen actions ---
    void enterSetMode();
    void handleDigit(int digit);
    void nextField();
    void confirmAndSave();
    void cancelEdit();
    void toggleEnabled();

    // --- Trigger detection & ringing screen actions ---
    void checkTrigger(int currentHour, int currentMinute, int currentSecond);
    bool isTriggered() const;
    void stopAlarm();
    void snooze();

    // --- Getters for display ---
    bool isEnabled() const;
    bool isEditingHour() const;
    bool isEditingMinute() const;
    String getDisplayHHMM() const;

private:
    BuzzerManager& _buzzer;
    LEDManager& _led;

    int _hour;
    int _minute;
    bool _enabled;

    enum class EditField { NONE, HOUR, MINUTE };
    EditField _editField;
    int _editHour;
    int _editMinute;
    String _editBuffer;

    bool _triggered;
    bool _snoozing;
    int _snoozeCount;
    unsigned long _snoozeUntilMs;
    static const int MAX_SNOOZES = 3;
    static const unsigned long SNOOZE_DURATION_MS = 5UL * 60UL * 1000UL;
};
