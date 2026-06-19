#pragma once
#include <Arduino.h>

/**
 * Helpers
 * -------
 * Static utility class providing shared, stateless functions used across
 * the entire project: time string formatting, zero-padding, LCD text
 * centering, and a global debug-print wrapper.
 *
 * Kept as static methods (no instance state) so any module can call
 * Helpers::xxx() without needing to own or pass around an object.
 */
class Helpers {
public:
    // Enable/disable all debug output project-wide from one place.
    static const bool DEBUG_ENABLED = true;

    // Prints a debug message prefixed with a module tag, e.g.
    // Helpers::debugPrint("ALARM", "Alarm set to 07:30");
    // Output -> [ALARM] Alarm set to 07:30
    static void debugPrint(const char* moduleTag, const String& message);

    // Converts a number (0-99) into a 2-character zero-padded string.
    // 5 -> "05", 23 -> "23"
    static String pad2(int value);

    // Formats hours/minutes/seconds into "HH:MM:SS"
    static String formatHMS(int hours, int minutes, int seconds);

    // Formats hours/minutes into "HH:MM"
    static String formatHM(int hours, int minutes);

    // Formats day/month into "DD/MM"
    static String formatDM(int day, int month);

    // Centers a string within a given LCD width (default 16 chars) by
    // calculating left padding. Returns the padded string ready to print.
    static String centerText(const String& text, uint8_t width = 16);

    // Pads a string on the right with spaces so leftover characters from
    // a previous, longer string are overwritten on the LCD.
    static String padRight(const String& text, uint8_t width = 16);

    // Clamps an integer between min and max (inclusive).
    static int clampInt(int value, int minVal, int maxVal);
};
