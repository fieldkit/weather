#include <Wire.h>
#include <SPI.h>

#include <AmbientSensors.h>
#include <WeatherMeters.h>
#include <Check.h>

#include "debug.h"

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(100);
    }

    debugfln("test: Setup");

    fk::ModuleHardware hardware;
    fk::Check check(hardware);
    hardware.setup();
    if (!check.check()) {
        check.failed();
    }

    debugfln("test: Done");

    fk::AmbientSensors ambientSensors(hardware);
    fk::WeatherMeters meters(hardware.serialFlash);
    meters.setup();

    while (true) {
        if (meters.tick()) {
            auto time = meters.getTime();
            auto currentWind = meters.getCurrentWind();
            auto dailyWindGust = meters.getDailyWindGust();
            auto averageWind = meters.getTwoMinuteWindAverage();
            auto wind10mGust = meters.getLargestRecentWindGust();
            auto hourlyRain = meters.getHourlyRain();
            auto dailyRain = meters.getDailyRain();

            debugf("%lu,%d,%d,%d", millis(), time.hour, time.minute, time.second);
            debugf(",%f,%d", currentWind.speed, currentWind.direction.angle);
            debugf(",%f,%d", dailyWindGust.speed, dailyWindGust.direction.angle);
            debugf(",%f,%d", wind10mGust.speed, wind10mGust.direction.angle);
            debugf(",%f,%d", averageWind.speed, averageWind.direction.angle);
            debugfln(",%f,%f", hourlyRain, dailyRain);
        }
    }
}

void loop() {

}
