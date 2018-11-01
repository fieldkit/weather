#include <Arduino.h>
#include <Wire.h>

#include <weather_module.h>

extern "C" {

void setup() {
    Serial.begin(115200);

    pinMode(fk::WeatherHardware::PIN_PERIPH_ENABLE, OUTPUT);
    digitalWrite(fk::WeatherHardware::PIN_PERIPH_ENABLE, HIGH);

    while (!Serial && millis() < 2000) {
        delay(100);
    }

    if (!Serial) {
        // The call to end here seems to free up some memory.
        Serial.end();
        Serial5.begin(115200);
        log_uart_set(Serial5);
    }

    digitalWrite(fk::WeatherHardware::PIN_PERIPH_ENABLE, LOW);

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
        readings,
        0,
        fk_module_RequiredUptime_ALWAYS,
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
