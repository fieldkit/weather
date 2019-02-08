#include <Wire.h>
#include <SPI.h>

#include <watchdog.h>

#include <ambient_sensors.h>
#include <weather_meters.h>
#include <check.h>

#include "rtc.h"
#include "debug.h"
#include "board_definition.h"

void setup() {
    Serial.begin(115200);

    fk::board.disable_everything();

    while (!Serial && millis() < 2000) {
        delay(100);
    }

    debugfln("test: Setup");

    fk::clock.begin();

    fk::WeatherHardware hardware;
    fk::Check check(hardware);
    hardware.setup();
    if (!check.check()) {
        debugfln("test: FAILED");
        check.failed();
    }
    else {
        debugfln("test: PASSED");
    }

    fk::Leds leds;
    fk::Watchdog watchdog(leds);
    fk::AmbientSensors ambientSensors(hardware);
    fk::SerialFlashFileSystem flashFs{ watchdog };
    fk::FlashState<fk::WeatherState> flashState{ flashFs };
    fk::WeatherMeters meters(watchdog, flashState);

    meters.setup();

    while (true) {
        if (meters.tick()) {
            ambientSensors.takeReading();

            auto currentWind = meters.getCurrentWind();
            auto hourlyWindGust = meters.getHourlyWindGust();
            auto averageWind = meters.getTwoMinuteWindAverage();
            auto wind10mGust = meters.getLargestRecentWindGust();
            auto previousHourlyRain = meters.getPreviousHourlyRain();
            auto hourlyRain = meters.getHourlyRain();

            loginfof("Sensors", "sensors: wind(%f, %d) hourly(%f, %d) 10m(%f, %d) avg(%f, %d) rain(%f, %f)",
                     currentWind.speed, currentWind.direction.angle,
                     hourlyWindGust.speed, hourlyWindGust.direction.angle,
                     wind10mGust.speed, wind10mGust.direction.angle,
                     averageWind.speed, averageWind.direction.angle,
                     previousHourlyRain, hourlyRain);
        }
    }
}

void loop() {

}
