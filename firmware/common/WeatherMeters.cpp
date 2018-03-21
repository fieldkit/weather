#include "WeatherMeters.h"
#include "debug.h"
#include "rtc.h"

namespace fk {

static WeatherMeters *global_weather_meters{ nullptr };

void isr_wind();
void isr_rain();

void isr_wind() {
    global_weather_meters->wind();
}

void isr_rain() {
    global_weather_meters->rain();
}

WeatherMeters::WeatherMeters(SerialFlashChip &serialFlash) : flash(serialFlash) {
    global_weather_meters = this;
}

void WeatherMeters::wind() {
    auto now = millis();
    if (now - lastWindAt > 10) {
        windTicks++;
        lastWindAt = now;
    }
}

void WeatherMeters::rain() {
    auto now = millis();
    if (now - lastRainAt > 10) {
        auto minute = persistedState.time.minute();
        persistedState.lastHourOfRain[minute] += RainPerTick;
        lastRainAt = now;
    }
}

void WeatherMeters::setup() {
    pinMode(PinWindDir, INPUT);
    pinMode(PinWindSpeed, INPUT_PULLUP);
    pinMode(PinRain, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PinWindSpeed), isr_wind, FALLING);
    attachInterrupt(digitalPinToInterrupt(PinRain), isr_rain, FALLING);

    flash.setup();

    load();
}

float WeatherMeters::getHourlyRain() {
    auto total = 0.0f;
    for (auto i = 0; i < 60; ++i) {
        total += persistedState.lastHourOfRain[i];
    }
    return total;
}

WindReading WeatherMeters::getWindReading() {
    auto speed = getWindSpeed();
    auto direction = getWindDirection();
    return WindReading{ speed, direction };
}

WindReading WeatherMeters::getTwoMinuteWindAverage() {
    auto speedSum = 0.0f;
    auto directionSum = persistedState.lastTwoMinutesOfWind[0].direction.angle;
    auto d = persistedState.lastTwoMinutesOfWind[0].direction.angle;
    auto numberOfSamples = 0;
    for (auto i = 1 ; i < 120; i++) {
        if (persistedState.lastTwoMinutesOfWind[i].direction.angle != -1) {
            auto delta = persistedState.lastTwoMinutesOfWind[i].direction.angle - d;

            if (delta < -180) {
                d += delta + 360;
            }
            else if (delta > 180) {
                d += delta - 360;
            }
            else {
                d += delta;
            }

            directionSum += d;

            speedSum += persistedState.lastTwoMinutesOfWind[i].speed;

            numberOfSamples++;
        }
    }

    auto averageSpeed =  speedSum / (float)numberOfSamples;

    auto averageDirection = (int16_t)(directionSum / numberOfSamples);
    if (averageDirection >= 360) {
        averageDirection -= 360;
    }
    if (averageDirection < 0) {
        averageDirection += 360;
    }

    return WindReading{ averageSpeed, WindDirection{ -1, averageDirection } };
}

WindReading WeatherMeters::getLargestRecentWindGust() {
    auto gust = WindReading{};
    for (auto i = 0; i < 10; ++i) {
        if (persistedState.windGusts[i].strongerThan(gust)) {
            gust = persistedState.windGusts[i];
        }
    }
    return gust;
}

bool WeatherMeters::tick() {
    if (millis() - lastStatusTick < 500) {
        return false;
    }

    auto now = clock.now();

    // Is persisted state more than a few minutes from us?
    auto nowUnix = now.unixtime();
    auto savedUnix = persistedState.time.unixtime();
    auto difference = nowUnix > savedUnix ? nowUnix - savedUnix : savedUnix - nowUnix; // Avoid overflow.
    if (difference > 60 * 5) {
        debugfpln("Meters", "Zeroing persisted state! (%lu - %lu = %lu)", nowUnix, savedUnix, difference);
        persistedState = PersistedState{};
    }

    if (now.second() != persistedState.time.second()) {
        lastStatusTick = millis();

        currentWind = getWindReading();

        if (++persistedState.twoMinuteSecondsCounter > 119) {
            persistedState.twoMinuteSecondsCounter = 0;
        }

        if (now.minute() != persistedState.time.minute()) {
            FormattedTime nowFormatted{ now };
            debugfpln("Meters", "New Minute: %s", nowFormatted.toString());

            if (now.hour() != persistedState.time.hour()) {
                debugfpln("Meters", "New Hour");

                persistedState.previousHourlyRain = getHourlyRain();
                for (auto i = 0; i < 60; ++i) {
                    persistedState.lastHourOfRain[i] = 0;
                }

                persistedState.hourlyWindGust.zero();
                if (now.hour() < persistedState.time.hour()) {
                    debugfpln("Meters", "New Day (%d %d)", now.hour(), persistedState.time.hour());
                }
            }

            persistedState.lastHourOfRain[now.minute()] = 0;

            if (++persistedState.tenMinuteMinuteCounter > 9) {
                persistedState.tenMinuteMinuteCounter = 0;
            }

            persistedState.windGusts[persistedState.tenMinuteMinuteCounter].zero();
        }

        persistedState.lastTwoMinutesOfWind[persistedState.twoMinuteSecondsCounter] = currentWind;

        if (currentWind.strongerThan(persistedState.windGusts[persistedState.tenMinuteMinuteCounter])) {
            persistedState.windGusts[persistedState.tenMinuteMinuteCounter] = currentWind;
        }

        if (currentWind.strongerThan(persistedState.hourlyWindGust)) {
            persistedState.hourlyWindGust = currentWind;
        }

        persistedState.time = now;

        if (millis() - lastSave > 10000) {
            save();
            lastSave = millis();
        }

        return true;
    }

    return false;
}

float WeatherMeters::getWindSpeed() {
    auto delta = (millis() - lastWindCheck) / 1000.0;
    auto speed = windTicks / (float)delta;

    windTicks = 0;
    lastWindCheck = millis();

    return speed * WindPerTick;
}

WindDirection WeatherMeters::getWindDirection() {
    auto adc = (int16_t)analogRead(PinWindDir);
    auto direction = WindDirection {
        adc, windAdcToAngle(adc)
    };
    return direction;
}

void WeatherMeters::save() {
    flash.write(&persistedState, sizeof(PersistedState));
    debugfpln("Meters", "Save (hourly rain: %f)", getHourlyRain());
}

void WeatherMeters::load() {
    flash.read(&persistedState, sizeof(PersistedState));
    debugfpln("Meters", "Load (hourly rain: %f)", getHourlyRain());
}

int16_t WeatherMeters::windAdcToAngle(int16_t adc) {
    if (adc < 380) return 113;
    if (adc < 393) return 68;
    if (adc < 414) return 90;
    if (adc < 456) return 158;
    if (adc < 508) return 135;
    if (adc < 551) return 203;
    if (adc < 615) return 180;
    if (adc < 680) return 23;
    if (adc < 746) return 45;
    if (adc < 801) return 248;
    if (adc < 833) return 225;
    if (adc < 878) return 338;
    if (adc < 913) return 0;
    if (adc < 940) return 293;
    if (adc < 967) return 315;
    if (adc < 990) return 270;
    return -1;
}

}

