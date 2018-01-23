#include "ModuleHardware.h"

namespace fk {

void ModuleHardware::setup() {
    SPI.begin();
    Wire.begin();

    pinPeripheral(11, PIO_SERCOM);
    pinPeripheral(13, PIO_SERCOM);

    pinMode(13, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
}

void ModuleHardware::leds(bool on) {
    digitalWrite(A3, on ? HIGH : LOW);
    digitalWrite(A4, on ? HIGH : LOW);
    digitalWrite(A5, on ? HIGH : LOW);
}

}
