#include <Arduino.h>
#include <wiring_private.h>

#include "readings.h"

namespace fk {

WeatherReadings::WeatherReadings(ModuleHardware &hw, WeatherMeters &meters) : Task("Weather"), hw(&hw), meters(&meters) {
}

bool WeatherReadings::setup() {
    hw->setup();
    if (!meters->setup()) {
        return false;
    }

    hasSht31 = true;
    if (!hw->sht31Sensor.begin()) {
        hasSht31 = false;
        log("SHT31 FAILED");
    }

    hasMpl3115a2 = true;
    if (!hw->mpl3115a2Sensor.begin()) {
        hasMpl3115a2  =false;
        log("MPL3115A2 FAILED");
    }

    hasTsl2591 = true;
    if (!hw->tsl2591Sensor.begin()) {
        hasTsl2591 = false;
        log("TSL25911FN FAILED");
    }

    log("Ready");

    return true;
}

WeatherReadings::Sht31Reading WeatherReadings::getSht31Reading() {
    if (!hasSht31) {
        return Sht31Reading{};
    }

    auto temperature = hw->sht31Sensor.readTemperature();
    auto humidity = hw->sht31Sensor.readHumidity();
    return Sht31Reading{
        .temperature = temperature,
        .humidity = humidity,
    };
}

WeatherReadings::Mpl3115a2Reading WeatherReadings::getMpl3115a2Reading() {
    if (!hasMpl3115a2) {
        return Mpl3115a2Reading{};
    }

    auto pressurePascals = hw->mpl3115a2Sensor.getPressure();
    auto altitudeMeters = hw->mpl3115a2Sensor.getAltitude();
    auto temperature = hw->mpl3115a2Sensor.getTemperature();
    auto pressureInchesMercury = pressurePascals / 3377.0f;

    return Mpl3115a2Reading{
        .pressurePascals = pressureInchesMercury,
        .altitudeMeters = altitudeMeters,
        .temperature = temperature,
        .pressureInchesMercury = pressureInchesMercury,
    };
}

WeatherReadings::Tsl2591Reading WeatherReadings::getTsl2591Reading() {
    if (!hasTsl2591) {
        return Tsl2591Reading{};
    }

    auto fullLuminosity = hw->tsl2591Sensor.getFullLuminosity();
    auto ir = fullLuminosity >> 16;
    auto full = fullLuminosity & 0xFFFF;
    auto lux = hw->tsl2591Sensor.calculateLux(full, ir);

    return Tsl2591Reading{
        .fullLuminosity = fullLuminosity,
        .ir = ir,
        .full = full,
        .visible = full - ir,
        .lux = lux,
    };
}

TaskEval WeatherReadings::task() {
    Wire.begin();

    log("Sensors: begin...");

    auto sht31 = getSht31Reading();
    auto mpl3115a2 = getMpl3115a2Reading();
    auto tsl2591 = getTsl2591Reading();

    auto currentWind = meters->getCurrentWind();
    auto hourlyWindGust = meters->getHourlyWindGust();
    auto averageWind = meters->getTwoMinuteWindAverage();
    auto wind10mGust = meters->getLargestRecentWindGust();
    auto previousHourlyRain = meters->getPreviousHourlyRain();
    auto hourlyRain = meters->getHourlyRain();

    if (pending != nullptr) {
        auto readings = pending->readings;
        auto i = 0;
        readings[i++].value = sht31.temperature;
        readings[i++].value = sht31.humidity;

        readings[i++].value = mpl3115a2.temperature;
        readings[i++].value = mpl3115a2.pressurePascals;
        readings[i++].value = mpl3115a2.altitudeMeters;

        readings[i++].value = tsl2591.ir;
        readings[i++].value = tsl2591.visible;
        readings[i++].value = tsl2591.lux;

        readings[i++].value = currentWind.speed;
        readings[i++].value = currentWind.direction.angle;
        readings[i++].value = hourlyWindGust.speed;
        readings[i++].value = hourlyWindGust.direction.angle;
        readings[i++].value = wind10mGust.speed;
        readings[i++].value = wind10mGust.direction.angle;
        readings[i++].value = averageWind.speed;
        readings[i++].value = averageWind.direction.angle;

        readings[i++].value = previousHourlyRain;
        readings[i++].value = hourlyRain;

        pending->elapsed -= millis();

        for (auto j = 0; j < i; ++j) {
            readings[j].status = SensorReadingStatus::Done;
            readings[j].time = clock.getTime();
        }
    }

    log("Sensors: %fC %f%%, %fC %fpa %f\"/Hg %fm", sht31.temperature, sht31.humidity, mpl3115a2.temperature, mpl3115a2.pressurePascals, mpl3115a2.pressureInchesMercury, mpl3115a2.altitudeMeters);
    log("Sensors: ir(%lu) full(%lu) visible(%lu) lux(%f)", tsl2591.ir, tsl2591.full, tsl2591.visible, tsl2591.lux);
    log("Meters(WindCur): %f,%d", currentWind.speed, currentWind.direction.angle);
    log("Meters(WindHrly): %f,%d", hourlyWindGust.speed, hourlyWindGust.direction.angle);
    log("Meters(Wind10m): %f,%d", wind10mGust.speed, wind10mGust.direction.angle);
    log("Meters(WindAvg): %f,%d", averageWind.speed, averageWind.direction.angle);
    log("Meters(Rain): %f,%f", previousHourlyRain, hourlyRain);

    return TaskEval::done();
}

}
