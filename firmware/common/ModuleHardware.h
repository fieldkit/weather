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
    void setup() {
        SPI.begin();
        Wire.begin();

        pinPeripheral(11, PIO_SERCOM);
        pinPeripheral(13, PIO_SERCOM);

        pinMode(A3, OUTPUT);
        pinMode(A4, OUTPUT);
        pinMode(A5, OUTPUT);
    }

    void leds(bool on) {
        digitalWrite(A3, on ? HIGH : LOW);
        digitalWrite(A4, on ? HIGH : LOW);
        digitalWrite(A5, on ? HIGH : LOW);
    }

};

}

#endif
