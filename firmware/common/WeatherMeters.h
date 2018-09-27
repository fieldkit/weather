#ifndef FK_WEATHER_METERS_H_INCLUDED
#define FK_WEATHER_METERS_H_INCLUDED

#include <Arduino.h>
#include <RTClib.h>

#include <flash_storage.h>
#include <watchdog.h>

#include "ModuleHardware.h"

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

struct WeatherState : MinimumFlashState {
    /**
     * Time related counters.
     */
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

    /**
     * Return a DateTime for the UNIX time stamp in our state.
     */
    DateTime date_time() const {
        return DateTime{ time };
    }

    /**
     * Clear the weather related state. We have to do this because we need to
     * leave the MinimumFlashState alone.
     */
    void clear() {
        twoMinuteSecondsCounter = 0;
        tenMinuteMinuteCounter = 0;
        for (auto i = 0; i < 120; ++i) {
            lastTwoMinutesOfWind[i] = { };
        }
        for (auto i = 0; i < 10; ++i) {
            windGusts[i] = {  };
        }
        hourlyWindGust = { };
        previousHourlyRain = 0;
        for (auto i = 0; i < 60; ++i) {
            lastHourOfRain[i] = 0;
        }
    }
};

class WeatherMeters {
private:
    static constexpr float RainPerTick = 0.2794; // mm
    static constexpr float WindPerTick = 2.4; // km/hr
    static constexpr uint8_t PinWindDir = A1;
    static constexpr uint8_t PinWindSpeed = 10;
    static constexpr uint8_t PinRain = 6;

private:
    FlashState<WeatherState> *flash_;
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

public:
    WeatherMeters(Watchdog &watchdog, FlashState<WeatherState> &flash);

public:
    bool setup();
    void wind();
    void rain();
    bool tick();

public:
    WindReading getCurrentWind() {
        return currentWind;
    }
    WindReading getHourlyWindGust() {
        return flash_->state().hourlyWindGust;
    }
    float getPreviousHourlyRain() {
        return flash_->state().previousHourlyRain;
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

};

}

#endif
