#include "Check.h"

#include "debug.h"

namespace fk {

Check::Check(ModuleHardware &hw) : hw(&hw) {
}

bool Check::sht31() {
    if (!hw->sht31Sensor.begin()) {
        debugfln("test: SHT31 FAILED");
        return false;
    }

    debugfln("test: SHT31 PASSED");
    return true;
}

bool Check::mpl3115a2() {
    if (!hw->mpl3115a2Sensor.begin()) {
        debugfln("test: MPL3115A2 FAILED");
        return false;
    }

    debugfln("test: MPL3115A2 PASSED");
    return true;
}

bool Check::tsl2591() {
    if (!hw->tsl2591Sensor.begin()) {
        debugfln("test: TSL25911FN FAILED");
        return false;
    }

    debugfln("test: TSL25911FN PASSED");
    return true;
}

bool Check::flashMemory() {
    debugfln("test: Checking flash memory...");

    if (!hw->serialFlash.begin(ModuleHardware::PIN_FLASH_CS)) {
        debugfln("test: Flash memory FAILED");
        return false;
    }

    uint8_t buffer[256];
    hw->serialFlash.readID(buffer);
    if (buffer[0] == 0) {
        debugfln("test: Flash memory FAILED");
        return false;
    }

    uint32_t chipSize = hw->serialFlash.capacity(buffer);
    if (chipSize == 0) {
        debugfln("test: Flash memory FAILED");
        return false;
    }

    debugfln("test: Flash memory PASSED");
    return true;
}

bool Check::check() {
    auto failures = false;
    if (!flashMemory()) {
        failures = true;
    }
    if (!mpl3115a2()) {
        failures = true;
    }
    if (false) {
        if (!tsl2591()) {
            failures = true;
        }
    }
    if (!sht31()) {
        failures = true;
    }

    return !failures;
}

void Check::failed() {
    while (true) {
        hw->leds(false);
        delay(100);
        hw->leds(true);
        delay(100);
    }
}

}
