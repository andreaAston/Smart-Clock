#pragma once
#include <Arduino.h>
#include <DHT.h>

/**
 * SensorManager
 * -------------
 * Wraps the DHT11 temperature/humidity sensor. DHT11 can only be read
 * reliably every ~2 seconds, so this class self-throttles reads using
 * millis() and caches the last good values for instant, cheap getters.
 */
class SensorManager {
public:
    explicit SensorManager(uint8_t pin);
    void begin();

    // Call every loop(); internally decides when it's actually time to
    // perform a new sensor read.
    void update();

    float getTemperature() const;
    float getHumidity() const;

private:
    DHT _dht;
    float _temperature;
    float _humidity;
    unsigned long _lastReadMs;
    static const unsigned long READ_INTERVAL_MS = 2500;
};
