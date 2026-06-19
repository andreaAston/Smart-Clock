#pragma once
#include <Arduino.h>
#include "../ir/IRRemoteManager.h"
#include "../rtc/RTCManager.h"
#include "../sensors/SensorManager.h"
#include "../display/DisplayManager.h"
#include "../alarm/AlarmManager.h"
#include "../countdown/CountdownTimer.h"
#include "../stopwatch/StopwatchManager.h"

/**
 * MenuManager
 * -----------
 * Top-level finite state machine that owns the current screen/mode and
 * routes IR remote button presses to the correct sub-manager. This is
 * the ONLY class that knows about screen transitions; every other
 * manager only knows about its own internal state.
 *
 * Cycle: Clock -> Alarm(set) -> Countdown -> Stopwatch -> Clock
 *
 * Design note on PLAY: on the Clock and Alarm screens, PLAY advances
 * the cycle (per spec). On Countdown/Stopwatch, PLAY instead controls
 * Start/Pause/Resume for that screen (per spec), since those screens
 * need PLAY for their primary action. To still complete the cycle,
 * pressing PLAY on an idle/empty Countdown screen advances to
 * Stopwatch. PREV always means Back/Cancel; on Countdown, the first
 * PREV stops/resets an active timer, and a PREV while already idle
 * navigates back to Clock; on Stopwatch, PREV always returns to Clock.
 */
class MenuManager {
public:
    MenuManager(RTCManager& rtc, SensorManager& sensors, DisplayManager& display,
                AlarmManager& alarm, CountdownTimer& countdown, StopwatchManager& stopwatch);

    void begin();
    void handleInput(IRButton button);
    void update(); // call every loop(): ticks sub-systems and redraws display

private:
    enum class AppMode { CLOCK, ALARM_SET, COUNTDOWN, STOPWATCH, ALARM_TRIGGER };

    RTCManager& _rtc;
    SensorManager& _sensors;
    DisplayManager& _display;
    AlarmManager& _alarm;
    CountdownTimer& _countdown;
    StopwatchManager& _stopwatch;

    AppMode _mode;
    AppMode _modeBeforeTrigger;

    void handleClockInput(IRButton button);
    void handleAlarmSetInput(IRButton button);
    void handleCountdownInput(IRButton button);
    void handleStopwatchInput(IRButton button);
    void handleAlarmTriggerInput(IRButton button);

    void refreshDisplay();
    static int digitFromButton(IRButton button); // returns -1 if not a digit
    static const char* modeName(AppMode mode);
};
