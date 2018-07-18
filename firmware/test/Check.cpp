#include <SerialFlash.h>

#include "Check.h"

#include "debug.h"

namespace fk {

Check::Check(ModuleHardware &hw) : hw(&hw) {
}

static const char *serialFlashId2Chip(const unsigned char *id) {
    if (id[0] == 0xEF) {
        // Winbond
        if (id[1] == 0x40) {
            if (id[2] == 0x14) return "W25Q80BV";
            if (id[2] == 0x15) return "W25Q16DV";
            if (id[2] == 0x17) return "W25Q64FV";
            if (id[2] == 0x18) return "W25Q128FV";
            if (id[2] == 0x19) return "W25Q256FV";
        }
    }
    if (id[0] == 0x01) {
        // Spansion
        if (id[1] == 0x02) {
            if (id[2] == 0x16) return "S25FL064A";
            if (id[2] == 0x19) return "S25FL256S";
            if (id[2] == 0x20) return "S25FL512S";
        }
        if (id[1] == 0x20) {
            if (id[2] == 0x18) return "S25FL127S";
        }
        if (id[1] == 0x40) {
            if (id[2] == 0x15) return "S25FL116K";
        }
    }
    if (id[0] == 0xC2) {
        // Macronix
        if (id[1] == 0x20) {
            if (id[2] == 0x18) return "MX25L12805D";
        }
    }
    if (id[0] == 0x20) {
        // Micron
        if (id[1] == 0xBA) {
            if (id[2] == 0x20) return "N25Q512A";
            if (id[2] == 0x21) return "N25Q00AA";
        }
        if (id[1] == 0xBB) {
            if (id[2] == 0x22) return "MT25QL02GC";
        }
    }
    if (id[0] == 0xBF) {
        // SST
        if (id[1] == 0x25) {
            if (id[2] == 0x02) return "SST25WF010";
            if (id[2] == 0x03) return "SST25WF020";
            if (id[2] == 0x04) return "SST25WF040";
            if (id[2] == 0x41) return "SST25VF016B";
            if (id[2] == 0x4A) return "SST25VF032";
        }
        if (id[1] == 0x25) {
            if (id[2] == 0x01) return "SST26VF016";
            if (id[2] == 0x02) return "SST26VF032";
            if (id[2] == 0x43) return "SST26VF064";
        }
    }
    return "(unknown chip)";
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

    SerialFlashChip serialFlash;

    if (!serialFlash.begin(ModuleHardware::PIN_FLASH_CS)) {
        debugfln("test: Flash memory FAILED");
        return false;
    }

    uint8_t buffer[256];
    serialFlash.readID(buffer);
    if (buffer[0] == 0) {
        debugfln("test: Flash memory FAILED");
        return false;
    }

    auto chipSize = serialFlash.capacity(buffer);
    if (chipSize == 0) {
        debugfln("test: Flash memory FAILED");
        return false;
    }

    debugfln("Read Chip Identification:");
    debugf("  JEDEC ID:     %x %x %x", buffer[0], buffer[1], buffer[2]);
    debugfln("  Part Nummber: %s", serialFlashId2Chip(buffer));
    debugfln("  Memory Size:  %lu bytes Block Size: %lu bytes", chipSize, serialFlash.blockSize());

    if (false) {
        serialFlash.eraseAll();
        while (!serialFlash.ready()) {
            debugfln("test: Flash erasing...");
            delay(500);
        }
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
    if (!tsl2591()) {
        failures = true;
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
