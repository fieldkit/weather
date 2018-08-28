#include <Wire.h>
#include <SPI.h>

#include <watchdog.h>

#include <AmbientSensors.h>
#include <WeatherMeters.h>
#include <Check.h>

#include "rtc.h"
#include "debug.h"

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(100);
    }

    pinMode(8, OUTPUT);
    digitalWrite(8, HIGH);
    delay(500);

    debugfln("test: Setup");

    fk::clock.begin();

    fk::WeatherHardware hardware;
    fk::Check check(hardware);
    hardware.setup();
    if (!check.check()) {
        check.failed();
    }

    debugfln("test: Done");

    fk::Leds leds;
    fk::Watchdog watchdog(leds);
    fk::AmbientSensors ambientSensors(hardware);
    fk::WeatherMeters meters(watchdog);
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
