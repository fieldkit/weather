#include "WeatherMeters.h"
#include "debug.h"

namespace fk {

static WeatherMeters *global_weather_meters{ nullptr };

FlashStorage::FlashStorage(SerialFlashChip &serialFlash) : serialFlash(&serialFlash) {
}

static uint32_t crc16_table[16] = {
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
    0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
};

static uint16_t crc16_update(uint16_t start, uint8_t *p, size_t n) {
    uint16_t crc = start;
    uint16_t r;

    while (n-- > 0) {
        /* compute checksum of lower four bits of *p */
        r = crc16_table[crc & 0xF];
        crc = (crc >> 4) & 0x0FFF;
        crc = crc ^ r ^ crc16_table[*p & 0xF];

        /* now compute checksum of upper four bits of *p */
        r = crc16_table[crc & 0xF];
        crc = (crc >> 4) & 0x0FFF;
        crc = crc ^ r ^ crc16_table[(*p >> 4) & 0xF];

        p++;
    }

    return crc;
}

size_t FlashStorage::write(uint8_t recordId, void *ptr, size_t size) {
    uint8_t buffer[size + sizeof(uint16_t)];

    uint16_t oldHash{ 0 };
    serialFlash->read(0, (uint8_t *)&oldHash, sizeof(uint16_t));

    auto newHash = crc16_update(0, (uint8_t *)ptr, size);
    if (newHash == oldHash) {
        return size;
    }

    memcpy(buffer, &newHash, sizeof(uint16_t));
    memcpy(buffer + sizeof(uint16_t), ptr, size);

    serialFlash->eraseBlock(0);
    serialFlash->write(0, buffer, sizeof(buffer));

    return size;
}

size_t FlashStorage::read(uint8_t recordId, void *ptr, size_t size) {
    uint8_t buffer[size + sizeof(uint16_t)];
    uint16_t hash{ 0 };

    serialFlash->read(0, buffer, sizeof(buffer));

    memcpy(&hash, buffer, sizeof(uint16_t));

    auto expected = crc16_update(0, buffer + sizeof(uint16_t), size);
    if (expected != hash) {
        return 0;
    }

    memcpy(ptr, buffer + sizeof(uint16_t), size);
    return size;
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

        save();

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
    flash.write(0, &persistedState, sizeof(PersistedState));
    debugfpln("Meters", "Save (daily rain: %f)", persistedState.dailyRain);
}

void WeatherMeters::load() {
    flash.read(0, &persistedState, sizeof(PersistedState));
    debugfpln("Meters", "Load (daily rain: %f)", persistedState.dailyRain);
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

