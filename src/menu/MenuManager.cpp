#include "MenuManager.h"
#include "../utils/Helpers.h"

MenuManager::MenuManager(RTCManager& rtc, SensorManager& sensors, DisplayManager& display,
                          AlarmManager& alarm, CountdownTimer& countdown, StopwatchManager& stopwatch)
    : _rtc(rtc), _sensors(sensors), _display(display), _alarm(alarm),
      _countdown(countdown), _stopwatch(stopwatch),
      _mode(AppMode::CLOCK), _modeBeforeTrigger(AppMode::CLOCK) {}

void MenuManager::begin() {
    Serial.println(F("[MENU] Initialized in CLOCK mode"));
}

void MenuManager::handleInput(IRButton button) {
    if (button == IRButton::NONE) return;

    if (_mode == AppMode::ALARM_TRIGGER) {
        handleAlarmTriggerInput(button);
        return;
    }

    switch (_mode) {
        case AppMode::CLOCK:     handleClockInput(button); break;
        case AppMode::ALARM_SET: handleAlarmSetInput(button); break;
        case AppMode::COUNTDOWN: handleCountdownInput(button); break;
        case AppMode::STOPWATCH: handleStopwatchInput(button); break;
        default: break;
    }
}

void MenuManager::handleClockInput(IRButton button) {
    if (button == IRButton::PLAY) {
        _alarm.enterSetMode();
        _mode = AppMode::ALARM_SET;
        Helpers::debugPrint("MENU", String("Switched to ") + modeName(_mode));
    }
    // PREV/NEXT have no defined action on the main clock screen.
}

void MenuManager::handleAlarmSetInput(IRButton button) {
    int digit = digitFromButton(button);
    if (digit >= 0) {
        _alarm.handleDigit(digit);
        return;
    }

    switch (button) {
        case IRButton::PLAY:
            _alarm.confirmAndSave();
            _mode = AppMode::COUNTDOWN;
            Helpers::debugPrint("MENU", String("Switched to ") + modeName(_mode));
            break;
        case IRButton::NEXT:
            _alarm.nextField();
            break;
        case IRButton::PREV:
            _alarm.cancelEdit();
            _mode = AppMode::CLOCK;
            Helpers::debugPrint("MENU", String("Switched to ") + modeName(_mode));
            break;
        case IRButton::VOL_UP:
            _alarm.toggleEnabled();
            break;
        default: break;
    }
}

void MenuManager::handleCountdownInput(IRButton button) {
    int digit = digitFromButton(button);
    if (digit >= 0) {
        _countdown.addDigit(digit);
        return;
    }

    switch (button) {
        case IRButton::PLAY:
            // Nothing entered/started yet -> PLAY advances the cycle.
            if (_countdown.isIdleAndEmpty()) {
                _mode = AppMode::STOPWATCH;
                Helpers::debugPrint("MENU", String("Switched to ") + modeName(_mode));
            } else {
                _countdown.toggleStartPause();
            }
            break;
        case IRButton::NEXT:
            _countdown.clearDigits();
            break;
        case IRButton::PREV:
            // First PREV cancels an active/entered timer; a second PREV
            // (now idle and empty) navigates back to the clock screen.
            if (_countdown.isIdleAndEmpty()) {
                _mode = AppMode::CLOCK;
                Helpers::debugPrint("MENU", String("Switched to ") + modeName(_mode));
            } else {
                _countdown.stopAndReset();
            }
            break;
        default: break;
    }
}

void MenuManager::handleStopwatchInput(IRButton button) {
    switch (button) {
        case IRButton::PLAY:
            _stopwatch.toggleStartPause();
            break;
        case IRButton::NEXT:
            _stopwatch.reset();
            break;
        case IRButton::PREV:
            _mode = AppMode::CLOCK;
            Helpers::debugPrint("MENU", String("Switched to ") + modeName(_mode));
            break;
        default: break;
    }
}

void MenuManager::handleAlarmTriggerInput(IRButton button) {
    // PREV and digits are ignored while the alarm is ringing.
    if (button == IRButton::PLAY) {
        _alarm.stopAlarm();
        _mode = _modeBeforeTrigger;
        Helpers::debugPrint("MENU", "Alarm stopped by user");
    } else if (button == IRButton::NEXT) {
        _alarm.snooze();
        _mode = _modeBeforeTrigger;
        Helpers::debugPrint("MENU", "Alarm snoozed by user");
    }
}

void MenuManager::update() {
    _rtc.update();
    _sensors.update();
    _countdown.update();

    _alarm.checkTrigger(_rtc.getHour(), _rtc.getMinute(), _rtc.getSecond());

    if (_alarm.isTriggered() && _mode != AppMode::ALARM_TRIGGER) {
        _modeBeforeTrigger = _mode;
        _mode = AppMode::ALARM_TRIGGER;
        Helpers::debugPrint("MENU", "Switching to Alarm trigger screen");
    }

    refreshDisplay();
}

void MenuManager::refreshDisplay() {
    switch (_mode) {
        case AppMode::CLOCK: {
            String time = Helpers::formatHMS(_rtc.getHour(), _rtc.getMinute(), _rtc.getSecond());
            String temp = String((int)_sensors.getTemperature()) + "C";
            String date = Helpers::formatDM(_rtc.getDay(), _rtc.getMonth());
            String hum = String((int)_sensors.getHumidity()) + "%";
            _display.showClock(time, temp, date, hum, _alarm.isEnabled());
            break;
        }
        case AppMode::ALARM_SET:
            _display.showAlarmSet(_alarm.getDisplayHHMM(), _alarm.isEnabled(),
                                   _alarm.isEditingHour(), _alarm.isEditingMinute());
            break;
        case AppMode::COUNTDOWN:
            if (_countdown.isFinished()) {
                _display.showTimeUp();
            } else {
                _display.showCountdown(_countdown.getDisplayTime(), _countdown.getStatusSuffix());
            }
            break;
        case AppMode::STOPWATCH:
            _display.showStopwatch(_stopwatch.getDisplayTime(), _stopwatch.getStatusSuffix());
            break;
        case AppMode::ALARM_TRIGGER:
            _display.showAlarmTriggered();
            break;
    }
}

int MenuManager::digitFromButton(IRButton button) {
    // DIGIT_0..DIGIT_9 are contiguous in the enum, so a range check
    // avoids a 10-case switch.
    int value = static_cast<int>(button);
    int base = static_cast<int>(IRButton::DIGIT_0);
    int top = static_cast<int>(IRButton::DIGIT_9);
    if (value >= base && value <= top) {
        return value - base;
    }
    return -1;
}

const char* MenuManager::modeName(AppMode mode) {
    switch (mode) {
        case AppMode::CLOCK:         return "CLOCK";
        case AppMode::ALARM_SET:     return "ALARM_SET";
        case AppMode::COUNTDOWN:     return "COUNTDOWN";
        case AppMode::STOPWATCH:     return "STOPWATCH";
        case AppMode::ALARM_TRIGGER: return "ALARM_TRIGGER";
        default:                     return "UNKNOWN";
    }
}
