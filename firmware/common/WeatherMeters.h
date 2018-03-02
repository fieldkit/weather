#ifndef FK_WEATHER_METERS_H_INCLUDED
#define FK_WEATHER_METERS_H_INCLUDED

#include <Arduino.h>
#include <flash_storage.h>

namespace fk {

struct WindDirection {
    int16_t raw{ -1 };
    int16_t angle{ -1 };

    WindDirection() {
    }

    WindDirection(int16_t raw, int16_t angle) : raw(raw), angle(angle) {
    }

    void zero() {
        raw = -1;
        angle = -1;
    }
};

struct WindReading {
    float speed{ 0 };
    WindDirection direction;

    WindReading() {
    }

    WindReading(float speed, WindDirection direction) : speed(speed), direction(direction) {
    }

    void zero() {
        speed = 0;
        direction.zero();
    }

    bool strongerThan(WindReading &r) {
        return speed > r.speed;
    }
};

struct PersistedState {
    /**
     * Time related counters.
     */
    uint32_t time{ 0 };
    uint8_t second{ 0 };
    uint8_t minute{ 0 };
    uint8_t hour{ 0 };
    uint8_t twoMinuteSecondsCounter{ 0 };
    uint8_t tenMinuteMinuteCounter{ 0 };

    /**
     * Last 120 wind readings. Used to calculate an average.
     */
    WindReading lastTwoMinutesOfWind[120];
    /**
     * For the last 10 minutes the largest gust that occured during that minute.
     */
    WindReading windGusts[10];
    /*
     * The maximum gust of wind for the hour.
     */
    WindReading hourlyWindGust;

    /**
     * Amount of rain rolled over.
     */
    float previousHourlyRain{ 0 };

    /**
     * Stores the rainfall per minute over the last 60 minutes.
     */
    float lastHourOfRain[60] = { 0 };
};

struct DumbTime {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
};

class WeatherMeters {
private:
    static constexpr float RainPerTick = 0.2794; // mm
    static constexpr float WindPerTick = 2.4; // km/hr
    static constexpr uint8_t PinWindDir = A1;
    static constexpr uint8_t PinWindSpeed = 10;
    static constexpr uint8_t PinRain = 6;

private:
    uint32_t lastStatusTick{ 0 };
    uint32_t lastSave{ 0 };
    uint32_t lastRainAt{ 0 };
    uint32_t lastWindAt{ 0 };
    uint32_t lastWindCheck{ 0 };
    uint32_t windTicks{ 0 };

    /**
     * Wind recording that was just taken.
     */
    WindReading currentWind;
    PersistedState persistedState;
    FlashStorage flash;

public:
    WeatherMeters(SerialFlashChip &serialFlash);

public:
    void wind();
    void rain();
    void setup();
    bool tick();

public:
    DumbTime getTime() {
        return DumbTime{ persistedState.second, persistedState.minute, persistedState.hour };
    }
    WindReading getCurrentWind() {
        return currentWind;
    }
    WindReading getHourlyWindGust() {
        return persistedState.hourlyWindGust;
    }
    float getPreviousHourlyRain() {
        return persistedState.previousHourlyRain;
    }
    float getHourlyRain();
    WindReading getWindReading();
    WindReading getTwoMinuteWindAverage();
    WindReading getLargestRecentWindGust();

private:
    // Basically converting ticks per second to a wind speed.
    float getWindSpeed();
    WindDirection getWindDirection();
    int16_t windAdcToAngle(int16_t adc);
    void save();
    void load();

};

}

#endif
