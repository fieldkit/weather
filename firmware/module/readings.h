#ifndef FK_WEATHER_READINGS_H_INCLUDED
#define FK_WEATHER_READINGS_H_INCLUDED

#include <fk-module.h>

#include "module_hardware.h"
#include "weather_meters.h"

namespace fk {

class WeatherReadings : public Task {
private:
    WeatherHardware *hw;
    WeatherMeters *meters;
    PendingReadings *pending;
    bool hasSht31;
    bool hasMpl3115a2;
    bool hasTsl2591;

public:
    WeatherReadings(WeatherHardware &hw, WeatherMeters &meters);

public:
    TaskEval task() override;

public:
    bool setup();
    void begin(PendingReadings *p) {
        pending = p;
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
