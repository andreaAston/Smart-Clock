#include "IRRemoteManager.h"
#include <IRremote.hpp>

// NEC raw codes for the car MP3 remote, mapped to logical buttons.
namespace {
    const uint32_t CODE_0    = 0xE916FF00;
    const uint32_t CODE_1    = 0xF30CFF00;
    const uint32_t CODE_2    = 0xE718FF00;
    const uint32_t CODE_3    = 0xA15EFF00;
    const uint32_t CODE_4    = 0xF708FF00;
    const uint32_t CODE_5    = 0xE31CFF00;
    const uint32_t CODE_6    = 0xA55AFF00;
    const uint32_t CODE_7    = 0xBD42FF00;
    const uint32_t CODE_8    = 0xAD52FF00;
    const uint32_t CODE_9    = 0xB54AFF00;
    const uint32_t CODE_PREV = 0xBB44FF00;
    const uint32_t CODE_NEXT = 0xBF40FF00;
    const uint32_t CODE_PLAY = 0xBC43FF00;
    const uint32_t CODE_VOLDN = 0xF807FF00;
    const uint32_t CODE_VOLUP = 0xEA15FF00;
}

IRRemoteManager::IRRemoteManager(uint8_t recvPin) : _pin(recvPin) {}

void IRRemoteManager::begin() {
    // false = disable the LED feedback flash on the receiver module.
    IrReceiver.begin(_pin, false);
    Serial.println(F("[IR] Receiver initialized"));
}

IRButton IRRemoteManager::poll() {
    if (!IrReceiver.decode()) {
        return IRButton::NONE;
    }

    uint32_t code = IrReceiver.decodedIRData.decodedRawData;
    IRButton btn = mapCodeToButton(code);

    if (btn != IRButton::NONE) {
        Serial.print(F("[IR] Code received: 0x"));
        Serial.println(code, HEX);
    } else {
        Serial.print(F("[IR] Unmapped code: 0x"));
        Serial.println(code, HEX);
    }

    IrReceiver.resume();
    return btn;
}

IRButton IRRemoteManager::mapCodeToButton(uint32_t code) {
    switch (code) {
        case CODE_0: return IRButton::DIGIT_0;
        case CODE_1: return IRButton::DIGIT_1;
        case CODE_2: return IRButton::DIGIT_2;
        case CODE_3: return IRButton::DIGIT_3;
        case CODE_4: return IRButton::DIGIT_4;
        case CODE_5: return IRButton::DIGIT_5;
        case CODE_6: return IRButton::DIGIT_6;
        case CODE_7: return IRButton::DIGIT_7;
        case CODE_8: return IRButton::DIGIT_8;
        case CODE_9: return IRButton::DIGIT_9;
        case CODE_PREV: return IRButton::PREV;
        case CODE_NEXT: return IRButton::NEXT;
        case CODE_PLAY: return IRButton::PLAY;
        case CODE_VOLDN: return IRButton::VOL_DOWN;
        case CODE_VOLUP: return IRButton::VOL_UP;
        default: return IRButton::NONE;
    }
}
