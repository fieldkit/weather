#ifndef FK_WEATHER_READINGS_H_INCLUDED
#define FK_WEATHER_READINGS_H_INCLUDED

#include <fk-module.h>

#include "ModuleHardware.h"
#include "WeatherMeters.h"

namespace fk {

class WeatherReadings : public Task {
private:
    ModuleHardware *hw;
    WeatherMeters *meters;
    PendingSensorReading *pending;
    bool hasSht31;
    bool hasMpl3115a2;
    bool hasTsl2591;

public:
    WeatherReadings(ModuleHardware &hw, WeatherMeters &meters);

public:
    TaskEval task() override;

public:
    void setup();
    void begin(PendingSensorReading &p) {
        pending = &p;
        pending->elapsed = millis();
    }

private:
    struct Sht31Reading {
        float temperature;
        float humidity;
    };

    struct Mpl3115a2Reading {
        float pressurePascals;
        float altitudeMeters;
        float temperature;
        float pressureInchesMercury;
    };

    struct Tsl2591Reading {
        uint32_t fullLuminosity;
        uint32_t ir;
        uint32_t full;
        uint32_t visible;
        float lux;
    };

    Sht31Reading getSht31Reading();
    Mpl3115a2Reading getMpl3115a2Reading();
    Tsl2591Reading getTsl2591Reading();

};

};

#endif
