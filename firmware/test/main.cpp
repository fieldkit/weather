#include <Wire.h>
#include <SPI.h>

#include <alogging/alogging.h>

#include <watchdog.h>
#include <rtc.h>
#include <debug.h>
#include <board_definition.h>
#include <tuning.h>

#include <ambient_sensors.h>
#include <weather_meters.h>
#include <check.h>

using namespace fk;

constexpr const char LogName[] = "TEST";

using Log = SimpleLog<LogName>;

void setup() {
    Serial.begin(115200);

    board.disable_everything();

    while (!Serial && millis() < 2000) {
        delay(100);
    }

    board.enable_everything();

    Log::info("test: Setup");

    clock.begin();

    WeatherHardware hardware;
    Check check(hardware);
    hardware.setup();
    if (!check.check()) {
        Log::info("test: FAILED");
        check.failed();
    }
    else {
        Log::info("test: PASSED");
    }

    Leds leds;
    Watchdog watchdog(leds);
    AmbientSensors ambientSensors(hardware);
    SerialFlashFileSystem flashFs{ watchdog };
    FlashState<WeatherState> flashState{ flashFs };
    WeatherMeters meters(watchdog, flashState);

    if (!flashFs.initialize(board.flash_cs(), SuperBlockSize)) {
        Log::info("test: FAILED Flash unavailable.");
        check.failed();
    }

    if (!flashState.initialize()) {
        Log::info("test: FAILED Flash initialize failed.");
        check.failed();
    }

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

            Log::info("sensors: wind(%f, %d) hourly(%f, %d) 10m(%f, %d) avg(%f, %d) rain(%f, %f)",
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
