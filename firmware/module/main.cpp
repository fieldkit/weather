#include <Arduino.h>
#include <Wire.h>

#include <fk-module.h>

#include "readings.h"

namespace fk {

class WeatherModule : public Module {
private:
    TwoWireBus bus{ Wire4and3 };
    Delay delay{ 500 };
    WeatherReadings *weatherReadings;

public:
    WeatherModule(ModuleInfo &info, WeatherReadings &weatherReadings);

public:
    ModuleReadingStatus beginReading(PendingSensorReading &pending) override;
    ModuleReadingStatus readingStatus(PendingSensorReading &pending) override;
    void done(Task &task) override;

};

WeatherModule::WeatherModule(ModuleInfo &info, WeatherReadings &weatherReadings) :
    Module(bus, info), weatherReadings(&weatherReadings) {
}

ModuleReadingStatus WeatherModule::beginReading(PendingSensorReading &pending) {
    weatherReadings->begin(pending);
    push(delay); // This is to give us time to reply with the backoff. Should be avoidable?
    push(*weatherReadings);

    return ModuleReadingStatus{ 5000 };
}

ModuleReadingStatus WeatherModule::readingStatus(PendingSensorReading &pending) {
    return ModuleReadingStatus{};
}

void WeatherModule::done(Task &task) {
    resume();
}

}

extern "C" {

void setup() {
    Serial.begin(115200);

    while (!Serial && millis() < 2000) {
        delay(100);
    }

    debugfpln("Module", "Starting (%lu free)", fk_free_memory());

    fk::ModuleInfo info = {
        fk_module_ModuleType_SENSOR,
        8,
        18,
        "Weather",
        {
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
        },
        {
            {}, {}, {}, {}, {},
            {}, {}, {},
            {}, {}, {}, {}, {}, {}, {}, {},
            {}, {},
        },
    };

    fk::ModuleHardware hardware;
    fk::WeatherMeters meters(hardware.serialFlash);
    fk::WeatherReadings weatherReadings(hardware, meters);
    fk::WeatherModule module(info, weatherReadings);

    module.begin();

    fk_assert(hardware.serialFlash.begin(fk::ModuleHardware::PIN_FLASH_CS));

    weatherReadings.setup();

    while (true) {
        module.tick();
        meters.tick();

        delay(10);
    }
}

void loop() {
}

}
