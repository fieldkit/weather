#include <Arduino.h>
#include <wiring_private.h>

#include "readings.h"

namespace fk {

WeatherReadings::WeatherReadings(ModuleHardware &hw, WeatherMeters &meters) : Task("Weather"), hw(&hw), meters(&meters) {
}

void WeatherReadings::setup() {
    hw->setup();
    meters->setup();

    if (!hw->sht31Sensor.begin()) {
        log("SHT31 FAILED");
    }

    if (!hw->mpl3115a2Sensor.begin()) {
        log("MPL3115A2 FAILED");
    }

    if (!hw->tsl2591Sensor.begin()) {
        log("TSL25911FN FAILED");
    }

    log("Ready");
}

TaskEval WeatherReadings::task() {
    Wire.begin();

    auto shtTemperature = hw->sht31Sensor.readTemperature();
    auto shtHumidity = hw->sht31Sensor.readHumidity();

    auto pressurePascals = hw->mpl3115a2Sensor.getPressure();
    auto altitudeMeters = hw->mpl3115a2Sensor.getAltitude();
    auto mplTempCelsius = hw->mpl3115a2Sensor.getTemperature();
    auto pressureInchesMercury = pressurePascals / 3377.0;

    auto fullLuminosity = hw->tsl2591Sensor.getFullLuminosity();
    auto ir = fullLuminosity >> 16;
    auto full = fullLuminosity & 0xFFFF;
    auto lux = hw->tsl2591Sensor.calculateLux(full, ir);

    auto time = meters->getTime();
    auto currentWind = meters->getCurrentWind();
    auto dailyWindGust = meters->getDailyWindGust();
    auto averageWind = meters->getTwoMinuteWindAverage();
    auto wind10mGust = meters->getLargestRecentWindGust();
    auto hourlyRain = meters->getHourlyRain();
    auto dailyRain = meters->getDailyRain();

    if (pending != nullptr) {
        auto readings = pending->readings;
        auto i = 0;
        readings[i++].value = shtTemperature;
        readings[i++].value = shtHumidity;
        readings[i++].value = mplTempCelsius;
        readings[i++].value = pressurePascals;
        readings[i++].value = altitudeMeters;
        readings[i++].value = ir;
        readings[i++].value = full - ir;
        readings[i++].value = lux;

        readings[i++].value = currentWind.speed;
        readings[i++].value = currentWind.direction.angle;
        readings[i++].value = dailyWindGust.speed;
        readings[i++].value = dailyWindGust.direction.angle;
        readings[i++].value = wind10mGust.speed;
        readings[i++].value = wind10mGust.direction.angle;
        readings[i++].value = averageWind.speed;
        readings[i++].value = averageWind.direction.angle;
        readings[i++].value = hourlyRain;
        readings[i++].value = dailyRain;

        pending->elapsed -= millis();

        for (auto j = 0; j < i; ++j) {
            readings[j].status = SensorReadingStatus::Done;
            readings[j].time = clock.getTime();
        }
    }

    log("Sensors: %fC %f%%, %fC %fpa %f\"/Hg %fm", shtTemperature, shtHumidity, mplTempCelsius, pressurePascals, pressureInchesMercury, altitudeMeters);
    log("Sensors: ir(%lu) full(%lu) visible(%lu) lux(%f)", ir, full, full - ir, lux);
    log("Meters: %lu,%d,%d,%d", millis(), time.hour, time.minute, time.second);
    log("Meters: %f,%d", currentWind.speed, currentWind.direction.angle);
    log("Meters: %f,%d", dailyWindGust.speed, dailyWindGust.direction.angle);
    log("Meters: %f,%d", wind10mGust.speed, wind10mGust.direction.angle);
    log("Meters: %f,%d", averageWind.speed, averageWind.direction.angle);
    log("Meters(Rain): %f,%f", hourlyRain, dailyRain);

    return TaskEval::done();
}

}
