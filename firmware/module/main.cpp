#include <Arduino.h>
#include <Wire.h>

#include <fk-module.h>
#include "module_idle.h"

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

WeatherServices *WeatherModuleState::weatherServices_{ nullptr };

class TakeWeatherReadings : public WeatherModuleState {
public:
    const char *name() const override {
        return "TakeWeatherReadings";
    }

public:
    void task() override;
};

void TakeWeatherReadings::task() {
    weatherServices().weatherReadings->enqueued();

    while (simple_task_run(*weatherServices().weatherReadings)) {
        services().alive();
    }

    transit<ModuleIdle>();
}

class WeatherModule : public Module {
private:
    TwoWireBus bus{ Wire4and3 };
    ModuleHardware hardware_;
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
    ModuleReadingStatus beginReading(PendingSensorReading &pending) override;
    DeferredModuleState beginReadingState() override;
    ModuleReadingStatus readingStatus(PendingSensorReading &pending) override;
    void tick() override;

};

WeatherModule::WeatherModule(ModuleInfo &info) :
    Module(bus, info), meters_(watchdog()) {
}

ModuleReadingStatus WeatherModule::beginReading(PendingSensorReading &pending) {
    weatherReadings_.begin(pending);

    // taskQueue().append(delay_); // This is to give us time to reply with the backoff. Should be avoidable?
    // taskQueue().append(weatherReadings_);

    return ModuleReadingStatus{ 5000 };
}

fk::DeferredModuleState WeatherModule::beginReadingState() {
    return ModuleFsm::deferred<TakeWeatherReadings>();
}

ModuleReadingStatus WeatherModule::readingStatus(PendingSensorReading &pending) {
    return ModuleReadingStatus{};
}

void WeatherModule::tick() {
    meters_.tick();
    Module::tick();
}

}

extern "C" {

void setup() {
    Serial.begin(115200);

    while (!Serial && millis() < 2000) {
        delay(100);
    }

    firmware_version_set(FIRMWARE_GIT_HASH);
    firmware_build_set(FIRMWARE_BUILD);
    firmware_compiled_set(DateTime(__DATE__, __TIME__).unixtime());

    loginfof("Module", "Starting (%lu free)", fk_free_memory());

    fk::SensorInfo sensors[18] = {
        {"temp_1", "°C",},
        {"humidity", "%",},
        {"temp_2", "°C",},
        {"pressure", "pa",},
        {"altitude", "m",},

        {"light_ir", "",},
        {"light_visible", "",},
        {"light_lux", "",},

        {"wind_speed", "km/hr"},
        {"wind_dir", ""},
        {"wind_hr_speed", "km/hr"},
        {"wind_hr_dir", ""},
        {"wind_10m_gust", "km/hr"},
        {"wind_10m_dir", ""},
        {"wind_2m_gust", "km/hr"},
        {"wind_2m_dir", ""},

        {"rain_prev_hour", "mm"},
        {"rain_this_hour", "mm"},
    };

    fk::SensorReading readings[18];

    fk::ModuleInfo info = {
        fk_module_ModuleType_SENSOR,
        8,
        18,
        1,
        "Weather",
        "fk-weather",
        sensors,
        readings
    };

    fk::WeatherModule module(info);

    module.begin();

    if (!module.readings().setup()) {
    }

    while (true) {
        module.tick();

        delay(10);
    }
}

void loop() {
}

}
