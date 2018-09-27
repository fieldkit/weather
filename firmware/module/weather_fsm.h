#ifndef FK_WEATHER_FSM_H_INCLUDED
#define FK_WEATHER_FSM_H_INCLUDED

#include <fk-module.h>
#include "readings.h"

namespace fk {

struct WeatherServices {
    WeatherReadings *weatherReadings;

    WeatherServices(WeatherReadings *weatherReadings) : weatherReadings(weatherReadings) {
    }
};

class WeatherModuleState : public ModuleServicesState {
private:
    static WeatherServices *weatherServices_;

public:
    static WeatherServices &weatherServices() {
        fk_assert(weatherServices_ != nullptr);
        return *weatherServices_;
    }

    static void atlasServices(WeatherServices &newServices) {
        weatherServices_ = &newServices;
    }

};

}

#endif
