#include <Arduino.h>

class AnalogSampler {
    static constexpr uint16_t SampleRate = 4096;

private:
    uint8_t pin{ A1 };
    uint16_t actualSampleRate{ 0 };
    short samples[SampleRate];
    size_t index{ 0 };

public:
    AnalogSampler();

public:
    void setup();
    void log();
    void timerHandler();

private:
    uint8_t sample();
    void configureAdc();
    void configureTimer();

};
