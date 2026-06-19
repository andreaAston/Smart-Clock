#include "SensorManager.h"

SensorManager::SensorManager(uint8_t pin)
    : _dht(pin, DHT11), _temperature(0.0f), _humidity(0.0f), _lastReadMs(0) {}

void SensorManager::begin() {
    _dht.begin();
    Serial.println(F("[SENSOR] DHT11 initialized"));
}

void SensorManager::update() {
    unsigned long now = millis();
    if (now - _lastReadMs < READ_INTERVAL_MS) {
        return; // too soon - keep last cached values
    }
    _lastReadMs = now;

    float t = _dht.readTemperature();
    float h = _dht.readHumidity();

    if (isnan(t) || isnan(h)) {
        Serial.println(F("[SENSOR] Read failed - keeping last values"));
        return;
    }

    _temperature = t;
    _humidity = h;

    Serial.print(F("[SENSOR] Temp: "));
    Serial.print(_temperature);
    Serial.print(F("C  Humidity: "));
    Serial.print(_humidity);
    Serial.println(F("%"));
}

float SensorManager::getTemperature() const { return _temperature; }
float SensorManager::getHumidity() const { return _humidity; }
