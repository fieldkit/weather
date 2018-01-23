#ifndef FK_MODULE_HARDWARE_H_INCLUDED
#define FK_MODULE_HARDWARE_H_INCLUDED

#include <Arduino.h>
#include <wiring_private.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_SHT31.h>
#include <SerialFlash.h>

namespace fk {

class ModuleHardware {
public:
    static constexpr uint8_t PIN_FLASH_CS = 5;

public:
    Adafruit_SHT31 sht31Sensor;
    Adafruit_MPL3115A2 mpl3115a2Sensor;
    Adafruit_TSL2591 tsl2591Sensor{ 2591 };
    SerialFlashChip serialFlash;

public:
    void setup();
    void leds(bool on);

};

}

#endif
