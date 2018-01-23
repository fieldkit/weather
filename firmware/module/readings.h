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

};

};

#endif
