#ifndef FK_WEATHER_MODULE_H_INCLUDED
#define FK_WEATHER_MODULE_H_INCLUDED

#include <fk-module.h>
#include "weather_fsm.h"

namespace fk {

class TakeWeatherReadings : public WeatherModuleState {
public:
    const char *name() const override {
        return "TakeWeatherReadings";
    }

public:
    void task() override;
};

class WeatherModule : public Module<WeatherState> {
private:
    TwoWireBus bus{ Wire4and3 };
    WeatherHardware hardware_;
    WeatherMeters meters_;
    WeatherReadings weatherReadings_{ hardware_, meters_ };
    WeatherServices weatherServices_{
        &weatherReadings_
    };

public:
    WeatherReadings &readings() {
        return weatherReadings_;
    }

public:
    WeatherModule(ModuleInfo &info);

public:
    fk::ModuleStates states() override {
        return {
            fk::ModuleFsm::deferred<fk::ConfigureModule>(),
            fk::ModuleFsm::deferred<TakeWeatherReadings>()
        };
    }

    void tick() override;

};

}

#endif
