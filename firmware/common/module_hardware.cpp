#include <SPI.h>

#include "module_hardware.h"
#include "board_definition.h"

namespace fk {

bool WeatherHardware::setup() {
    SPI.begin();
    Wire.begin();

    pinMode(FK_WEATHER_PIN_PERIPHERALS_ENABLE, OUTPUT);
    digitalWrite(FK_WEATHER_PIN_PERIPHERALS_ENABLE, LOW);

    pinPeripheral(11, PIO_SERCOM);
    pinPeripheral(13, PIO_SERCOM);

    pinMode(13, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);

    delay(2000);

    digitalWrite(FK_WEATHER_PIN_PERIPHERALS_ENABLE, HIGH);

    return true;
}

void WeatherHardware::leds(bool on) {
    digitalWrite(A3, on ? HIGH : LOW);
    digitalWrite(A4, on ? HIGH : LOW);
    digitalWrite(A5, on ? HIGH : LOW);
}

}
