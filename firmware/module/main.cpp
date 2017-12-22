#include <Arduino.h>
#include <Wire.h>

#include <fk-module.h>

#include "readings.h"

namespace fk {

class WeatherModule : public Module {
private:
    WeatherReadings *weatherReadings;

public:
    WeatherModule(ModuleInfo &info, WeatherReadings &weatherReadings);

public:
    void beginReading(SensorReading *readings) override;

};

WeatherModule::WeatherModule(ModuleInfo &info, WeatherReadings &weatherReadings) :
    Module(info), weatherReadings(&weatherReadings) {
}

void WeatherModule::beginReading(SensorReading *readings) {
    weatherReadings->begin(readings);
    push(*weatherReadings);
}

}

extern "C" {

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(100);
    }

    debugfpln("Module", "Starting (%lu free)", fk_free_memory());

    fk::ModuleInfo info = {
        8,
        12,
        "Weather",
        {
            {"Temp #1", "°C",},
            {"Humidity", "%",},
            {"Temp #2", "°C",},
            {"Pressure", "pa",},
            {"Altitude", "m",},
            {"Ambient IR", "",},
            {"Ambient Visible", "",},
            {"Ambient Lux", "",},
            {"IMU Cal", ""},
            {"IMU Orien X", ""},
            {"IMU Orien Y", ""},
            {"IMU Orien Z", ""},
        },
        {
            {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
        },
    };

    fk::WeatherReadings weatherReadings;
    fk::WeatherModule module(info, weatherReadings);
    uint32_t idleStart = 0;

    module.begin();

    weatherReadings.setup();

    while (true) {
        module.tick();

        if (module.isIdle()) {
            if (idleStart > 0 && millis() - idleStart > 1000) {
                module.beginReading(nullptr);
                idleStart = 0;
            } else if (idleStart == 0) {
                idleStart = millis();
            }
        }
        else {
            idleStart = 0;
        }

        delay(10);
    }
}

void loop() {
}

}
