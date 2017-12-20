#include <Wire.h>
#include <SPI.h>
#include <wiring_private.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_SHT31.h>
#include <SerialFlash.h>

const uint8_t PIN_FLASH_CS = 5;

Adafruit_TSL2591 tsl(2591);

TwoWire myWire1(&sercom1, 11, 13);
TwoWire myWire2(&sercom2, 4, 3);

class Adafruit_TSL25911FN {
private:
    uint8_t address;

private:
    uint8_t read8(uint8_t a) {
        Wire.beginTransmission(address);
        Wire.write(a);
        Wire.endTransmission(true);

        Wire.requestFrom((uint8_t)address, (uint8_t)1);
        return Wire.read();
    }

public:
    Adafruit_TSL25911FN(uint8_t address) {
        this->address = address;
    }

   bool begin() {
       if (tsl.begin()) {
           return true;
       }
       uint8_t value = read8(0x12);
       Serial.println(value, HEX);
       return value == 0x50;
   }
};

class Check {
private:
    const char * id2chip(const unsigned char *id) {
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

public:
    void setup() {
        SPI.begin();
        Wire.begin();

        myWire1.begin();
        myWire2.begin();

        pinPeripheral(11, PIO_SERCOM);
        pinPeripheral(13, PIO_SERCOM);
        pinPeripheral(4, PIO_SERCOM_ALT);
        pinPeripheral(3, PIO_SERCOM_ALT);

        pinMode(13, OUTPUT);
        pinMode(A3, OUTPUT);
        pinMode(A4, OUTPUT);
        pinMode(A5, OUTPUT);
    }

    void sht31() {
        Adafruit_SHT31 sensor;
        if (!sensor.begin()) {
            Serial.println("test: SHT31 FAILED");
        }
        else {
            Serial.println("test: SHT31 PASSED");
            Serial.println(sensor.readTemperature());
            Serial.println(sensor.readHumidity());
        }
    }

    void mpl3115a2() {
        Adafruit_MPL3115A2 sensor;
        if (!sensor.begin()) {
            Serial.println("test: MPL3115A2 FAILED");
        }
        else {
            Serial.println("test: MPL3115A2 PASSED");
        }
    }

    void tsl2591() {
        Adafruit_TSL25911FN tsl(0x29);

        if (!tsl.begin()) {
            Serial.println("test: TSL25911FN FAILED");
        }
        else {
            Serial.println("test: TSL25911FN PASSED");
        }
    }

    void bno055() {
        Adafruit_BNO055 bno(55, BNO055_ADDRESS_A, &myWire1);

        if (!bno.begin()) {
            Serial.println("test: BNO055 FAILED");
        }
        else {
            Serial.println("test: BNO055 PASSED");
        }
    }

    void max4466() {
        pinMode(A1, INPUT);

        while (true) {
            float value = analogRead(A1);
            Serial.println(value);
            delay(100);
        }
    }

    void flashMemory() {
        Serial.println("test: Checking flash memory...");

        if (!SerialFlash.begin(PIN_FLASH_CS)) {
            Serial.println("test: Flash memory FAILED");
            return;
        }

        uint8_t buffer[256];

        SerialFlash.readID(buffer);
        if (buffer[0] == 0) {
            Serial.println("test: Flash memory FAILED");
            return;
        }

        uint32_t chipSize = SerialFlash.capacity(buffer);
        if (chipSize == 0) {
            Serial.println("test: Flash memory FAILED");
            return;
        }

        Serial.println("Read Chip Identification:");
        Serial.print("  JEDEC ID:     ");
        Serial.print(buffer[0], HEX);
        Serial.print(" ");
        Serial.print(buffer[1], HEX);
        Serial.print(" ");
        Serial.println(buffer[2], HEX);
        Serial.print("  Part Nummber: ");
        Serial.println(id2chip(buffer));
        Serial.print("  Memory Size:  ");
        Serial.print(chipSize);
        Serial.println(" bytes");
        Serial.print("  Block Size:   ");
        uint32_t blockSize = SerialFlash.blockSize();
        Serial.print(blockSize);
        Serial.println(" bytes");

        Serial.println("test: Flash memory PASSED");
    }

    void leds(bool on) {
        digitalWrite(13, on ? HIGH : LOW);
        digitalWrite(A3, on ? HIGH : LOW);
        digitalWrite(A4, on ? HIGH : LOW);
        digitalWrite(A5, on ? HIGH : LOW);
    }

};

void advancedRead(void) {
    // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
    // That way you can do whatever math and comparisons you want!
    uint32_t lum = tsl.getFullLuminosity();
    uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;
    Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
    Serial.print(F("IR: ")); Serial.print(ir);  Serial.print(F("  "));
    Serial.print(F("Full: ")); Serial.print(full); Serial.print(F("  "));
    Serial.print(F("Visible: ")); Serial.print(full - ir); Serial.print(F("  "));
    Serial.print(F("Lux: ")); Serial.println(tsl.calculateLux(full, ir));
}

void setup() {
    Serial.begin(115200);

    Check check;
    check.setup();
    check.leds(false);

    while (!Serial) {
        delay(100);
    }

    Serial.println("test: Begin");

    /*
    Serial.print("Address: ");
    Serial.println(MPL3115A2_ADDRESS, HEX);

    {
        uint8_t value = read8(MPL3115A2_WHOAMI);
        Serial.println(value);
    }

    {
        uint8_t value = read8(MPL3115A2_REGISTER_STATUS);
        Serial.println(value);
    }

    while (true) {
    delay(100);
    }
    */

    while (true) {
        check.leds(false);

        check.flashMemory();
        check.bno055();
        check.mpl3115a2();
        check.tsl2591();
        check.sht31();

        advancedRead();

        check.leds(true);
        delay(500);
    }
    // check.max4466();

    Serial.println("test: Done");

    delay(100);
}

void loop() {

}
