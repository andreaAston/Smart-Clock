# Smart Clock — Arduino Uno (Clock / Alarm / Countdown / Stopwatch)

A modular embedded project for Arduino Uno with a DS1302 RTC, DHT11 sensor,
16x2 I2C LCD, IR remote control, buzzer, and status LED.

## 1. Hardware wiring

| Component        | Arduino Pin | Notes                  |
|-------------------|-------------|------------------------|
| IR Receiver       | D3          | Signal pin             |
| Buzzer            | D4          | Active buzzer          |
| Green LED         | D13         | With 220Ω resistor     |
| DHT11             | D2          | Data pin               |
| DS1302 DAT        | D7          | Data                   |
| DS1302 CLK        | D6          | Clock                  |
| DS1302 RST        | D8          | Reset/CE                |
| LCD I2C SDA       | A4          | I2C data               |
| LCD I2C SCL       | A5          | I2C clock              |
| LCD I2C address   | 0x27        | —                      |

## 2. Setting up VS Code + PlatformIO

1. **Install VS Code**
   Download and install from https://code.visualstudio.com/

2. **Install the PlatformIO IDE extension**
   - Open VS Code
   - Go to the Extensions panel (`Ctrl+Shift+X` / `Cmd+Shift+X`)
   - Search for **"PlatformIO IDE"** and click Install
   - Wait for it to finish installing (it installs its own Python toolchain
     in the background — this can take a few minutes the first time)
   - Restart VS Code when prompted

3. **Open this project**
   - In VS Code: `File → Open Folder...`
   - Select the `SmartClockProject` folder (the one containing
     `platformio.ini`)
   - PlatformIO will detect `platformio.ini` automatically and show a
     PlatformIO icon (alien head) in the left sidebar

4. **Let PlatformIO install the libraries**
   - The `lib_deps` section in `platformio.ini` already lists every
     library this project needs:
     - `IRremote` (IR remote decoding)
     - `DHT sensor library` + `Adafruit Unified Sensor` (DHT11)
     - `LiquidCrystal_I2C` (16x2 I2C LCD)
     - `Rtc by Makuna` (DS1302 RTC)
   - PlatformIO downloads these automatically the first time you build —
     you don't need to install anything manually via Library Manager.

5. **Build the project**
   - Click the PlatformIO icon in the sidebar → **PROJECT TASKS** →
     `env:uno` → **General** → **Build** (checkmark icon in the status bar)
   - Or just press the ✓ (checkmark) icon in the bottom blue status bar

6. **Connect your Arduino Uno and upload**
   - Plug the Uno in via USB
   - Click the → (right arrow) icon in the bottom status bar (**Upload**)
   - PlatformIO auto-detects the COM port; if it picks the wrong one, set
     `upload_port = COM3` (Windows) or `/dev/ttyUSB0` (Linux/Mac) under
     `[env:uno]` in `platformio.ini`

7. **Open the Serial Monitor**
   - Click the plug icon 🔌 in the bottom status bar, or run
     `PlatformIO: Serial Monitor` from the command palette
   - Baud rate is already set to `9600` in `platformio.ini`
   - You'll see tagged debug logs like:
     ```
     [RTC] Initialized
     [MENU] Switched to ALARM_SET
     [ALARM] Alarm set to 07:30
     [COUNTDOWN] Started
     ```

## 3. First boot — setting the RTC time

The DS1302 has no internal way to be set from the IR remote in this build.
On first boot, if the RTC has never been set (or its backup battery died),
`RTCManager` will default it to `2026-01-01 00:00:00` so the system still
boots cleanly.

To set the real date/time, temporarily call this once in `setup()` inside
`src/main.cpp`, upload, then remove/comment it out so it doesn't reset the
time on every reboot:

```cpp
rtc.setDateTime(2026, 6, 19, 14, 30, 0); // year, month, day, hour, min, sec
```

## 4. Project structure

```
src/
  main.cpp                 - wires hardware pins + main loop
  display/DisplayManager   - 16x2 I2C LCD screens, flicker-free updates
  alarm/AlarmManager        - alarm set/edit/trigger/snooze logic
  countdown/CountdownTimer  - countdown timer state machine
  stopwatch/StopwatchManager- stopwatch state machine
  menu/MenuManager          - central FSM, routes IR input to the active screen
  ir/IRRemoteManager        - decodes IR NEC codes into logical buttons
  rtc/RTCManager            - DS1302 wrapper
  sensors/SensorManager     - DHT11 wrapper
  hardware/BuzzerManager    - active buzzer (beep + repeating pattern)
  hardware/LEDManager       - status LED (steady + non-blocking blink)
  utils/Helpers             - shared time-formatting/debug helpers
```

## 5. Button map (PLAY / PREV / NEXT cycle)

- **PLAY** — on Clock: go to Alarm set. On Alarm set: save & go to
  Countdown. On Countdown/Stopwatch: Start/Pause/Resume (per spec); if
  Countdown is idle and empty, PLAY instead advances to Stopwatch.
- **PREV** — always Back/Cancel. On Countdown, the first press
  stops/resets an active timer; press again (now idle) to leave to Clock.
  On Stopwatch, always returns to Clock immediately.
- **NEXT** — context-dependent: switches Hour/Minute field on Alarm set,
  clears entered digits on Countdown, resets on Stopwatch.
- **Number keys (0–9)** — enter digits on Alarm set / Countdown screens.
- **VOL+** — toggles Alarm ON/OFF while on the Alarm set screen.
- **During an alarm ring:** PLAY stops it, NEXT snoozes (5 min, up to 3 times).

All debug activity is printed to Serial at 9600 baud, tagged by module
(`[RTC]`, `[ALARM]`, `[COUNTDOWN]`, `[STOPWATCH]`, `[MENU]`, `[IR]`,
`[DISPLAY]`, `[SENSOR]`, `[BUZZER]`, `[LED]`).
