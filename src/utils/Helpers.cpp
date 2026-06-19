#include "Helpers.h"

void Helpers::debugPrint(const char* moduleTag, const String& message) {
    if (!DEBUG_ENABLED) return;
    Serial.print('[');
    Serial.print(moduleTag);
    Serial.print("] ");
    Serial.println(message);
}

String Helpers::pad2(int value) {
    String result = "";
    if (value < 10) {
        result += '0';
    }
    result += String(value);
    return result;
}

String Helpers::formatHMS(int hours, int minutes, int seconds) {
    return pad2(hours) + ":" + pad2(minutes) + ":" + pad2(seconds);
}

String Helpers::formatHM(int hours, int minutes) {
    return pad2(hours) + ":" + pad2(minutes);
}

String Helpers::formatDM(int day, int month) {
    return pad2(day) + "/" + pad2(month);
}

String Helpers::centerText(const String& text, uint8_t width) {
    int len = text.length();
    if (len >= width) {
        // Truncate if too long - never let it wrap or corrupt the LCD row.
        return text.substring(0, width);
    }
    int totalPadding = width - len;
    int leftPadding = totalPadding / 2;

    String result = "";
    for (int i = 0; i < leftPadding; i++) {
        result += ' ';
    }
    result += text;

    // Pad the remainder on the right so old characters get overwritten.
    while (result.length() < width) {
        result += ' ';
    }
    return result;
}

String Helpers::padRight(const String& text, uint8_t width) {
    String result = text;
    if (result.length() > width) {
        return result.substring(0, width);
    }
    while (result.length() < width) {
        result += ' ';
    }
    return result;
}

int Helpers::clampInt(int value, int minVal, int maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}
