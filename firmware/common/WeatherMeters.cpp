#include "WeatherMeters.h"
#include "debug.h"

namespace fk {

static WeatherMeters *global_weather_meters{ nullptr };

static constexpr char FileName[] = "storage.bin";

FlashStorage::FlashStorage(SerialFlashChip &serialFlash) : serialFlash(&serialFlash) {
}

size_t FlashStorage::write(uint8_t recordId, void *ptr, size_t size) {
    if (createNew) {
        if (!SerialFlash.create(FileName, size)) {
            debugfpln("Flash", "Unable to create");
        }
    }
    auto file = serialFlash->open(FileName);
    if (file) {
        file.seek(0);
        file.write(ptr, size);
        debugfpln("Flash", "Wrote %d bytes", size);
    }
    return 0;
}

size_t FlashStorage::read(uint8_t recordId, void *ptr, size_t size) {
    if (!serialFlash->exists(FileName)) {
        debugfpln("Flash", "No file found");
        return 0;
    }
    auto file = serialFlash->open(FileName);
    if (file) {
        if (file.size() != size) {
            debugfpln("Flash", "File size mismatch %lu != %d", file.size(), size);
            serialFlash->remove(FileName);
            return 0;
        }

        file.read(ptr, size);

        createNew = false;

        debugfpln("Flash", "Read %d bytes", size);
    }

    return 0;
}

void PersistedState::save(FlashStorage &flash) {
    flash.write(0, this, sizeof(PersistedState));
}

void PersistedState::load(FlashStorage &flash) {
    flash.read(0, this, sizeof(PersistedState));
}

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
        persistedState.dailyRain += RainPerTick;
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

    persistedState.load(flash);
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
    if (millis() - lastStatusTick > 1000) {
        currentWind = getWindReading();

        lastStatusTick = millis();

        if (++twoMinuteSecondsCounter > 119) {
            twoMinuteSecondsCounter = 0;
        }

        if (++second > 59) {
            second = 0;
            if (++minute > 59) {
                minute = 0;
                if (++hour > 23) {
                    hour = 0;
                    persistedState.dailyRain = 0;
                    persistedState.dailyWindGust.zero();
                }
            }

            persistedState.lastHourOfRain[minute] = 0;

            if (++tenMinuteMinuteCounter > 9) {
                tenMinuteMinuteCounter = 0;
            }

            persistedState.windGusts[tenMinuteMinuteCounter].zero();
        }

        persistedState.lastTwoMinutesOfWind[twoMinuteSecondsCounter] = currentWind;

        if (currentWind.strongerThan(persistedState.windGusts[tenMinuteMinuteCounter])) {
            persistedState.windGusts[tenMinuteMinuteCounter] = currentWind;
        }
        if (currentWind.strongerThan(persistedState.dailyWindGust)) {
            persistedState.dailyWindGust = currentWind;
        }

        persistedState.save(flash);

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

