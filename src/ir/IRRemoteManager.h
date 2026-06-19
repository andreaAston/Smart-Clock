#pragma once
#include <Arduino.h>

/**
 * IRButton
 * --------
 * Logical button identifiers. This decouples the rest of the application
 * from raw IR hex codes - every other module only ever sees IRButton
 * values, never the underlying NEC protocol data.
 */
enum class IRButton {
    NONE,
    DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4,
    DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9,
    PREV, NEXT, PLAY, VOL_DOWN, VOL_UP
};

/**
 * IRRemoteManager
 * ----------------
 * Wraps the IRremote library for a car MP3 remote (NEC protocol).
 * Call begin() once in setup(), then poll() every loop() iteration.
 * poll() is non-blocking: it returns IRButton::NONE immediately if no
 * new signal has been received.
 */
class IRRemoteManager {
public:
    explicit IRRemoteManager(uint8_t recvPin);
    void begin();
    IRButton poll();

private:
    uint8_t _pin;
    static IRButton mapCodeToButton(uint32_t code);
};
