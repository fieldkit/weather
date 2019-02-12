#include <SPI.h>

#include "module_hardware.h"
#include "board_definition.h"

namespace fk {

bool WeatherHardware::setup() {
    return true;
}

void WeatherHardware::leds(bool on) {
    digitalWrite(13, on ? HIGH : LOW);
}

}
