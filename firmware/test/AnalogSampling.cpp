#include <Arduino.h>
#include <wiring_private.h>

#include "debug.h"
#include "AnalogSampling.h"

static AnalogSampler *global_sampler{ nullptr };

static bool adcIsBusy() {
    return ADC->STATUS.bit.SYNCBUSY;
}

static bool timerIsBusy() {
    return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

AnalogSampler::AnalogSampler() {
    global_sampler = this;
}

void AnalogSampler::setup() {
    analogReadResolution(12);

    configureAdc();
    configureTimer();
}

void AnalogSampler::configureAdc() {
    // Disable
    ADC->CTRLA.bit.ENABLE = 0x00;
    while (adcIsBusy()) {
    }

    // Configure
    ADC->CTRLB.bit.RESSEL = ADC_CTRLB_RESSEL_8BIT_Val;
    while (adcIsBusy()) {
    }

    ADC->CTRLB.bit.PRESCALER = ADC_CTRLB_PRESCALER_DIV8_Val;
    while (adcIsBusy()) {
    }

    while (adcIsBusy()) {
    }

    ADC->SAMPCTRL.reg = 0x1F;
    while (adcIsBusy()) {
    }

    // Set MUX
    pinPeripheral(pin, g_APinDescription[pin].ulPinType);

    while (adcIsBusy()) {
    }

    ADC->INPUTCTRL.bit.MUXPOS = g_APinDescription[pin].ulADCChannelNumber;

    // Enable
    ADC->CTRLA.bit.ENABLE = 0x01;
    while (adcIsBusy()) {
    }
}

void AnalogSampler::configureTimer() {
    // Disable
    TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
    while (timerIsBusy()) {
    }

    // Enable GCLK for TCC2 and TC5 (timer counter input clock)
    GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5)) ;
    while (GCLK->STATUS.bit.SYNCBUSY);

    // Reset
    TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
    while (timerIsBusy()) {
    }
    while (TC5->COUNT16.CTRLA.bit.SWRST) {
    }

    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;

    // Set TC5 mode as match frequency
    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;

    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_ENABLE;

    TC5->COUNT16.CC[0].reg = (uint16_t) (SystemCoreClock / SampleRate - 1);
    while (timerIsBusy()) {
    }

    actualSampleRate = SystemCoreClock / (TC5->COUNT16.CC[0].reg + 1);

    // Configure IRQ
    NVIC_DisableIRQ(TC5_IRQn);
    NVIC_ClearPendingIRQ(TC5_IRQn);
    NVIC_SetPriority(TC5_IRQn, 0x00);
    NVIC_EnableIRQ(TC5_IRQn);

    // Enable the TC5 IRQ
    TC5->COUNT16.INTENSET.bit.MC0 = 1;
    while (timerIsBusy()) {
    }

    // Enable
    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
    while (timerIsBusy()) {
    }
}

uint8_t AnalogSampler::sample() {
    uint8_t sample;

    while (adcIsBusy()) {
    }

    ADC->SWTRIG.bit.START = 1;

    while (ADC->INTFLAG.bit.RESRDY == 0) {
    }

    sample = ADC->RESULT.reg;

    while (adcIsBusy()) {
    }

    ADC->SWTRIG.bit.START = 0;

    return sample;
}

void AnalogSampler::timerHandler() {
    const auto adcResolution = 4096;
    const auto scale = 8192 / adcResolution;
    auto unscaled = (short)(analogRead(pin) - (adcResolution >> 1));
    auto sample = (short)((double)unscaled * scale);
    samples[index++] = sample;
    index = index % SampleRate;
}

void AnalogSampler::log() {
    debugfln("sensors: actualSampleRate(%d) analogIndex(%d)", actualSampleRate, index);
}

void TC5_Handler(void) {
    global_sampler->timerHandler();
    TC5->COUNT16.INTFLAG.bit.MC0 = 1;
}
