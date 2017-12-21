#include <Wire.h>
#include <SPI.h>
#include <wiring_private.h>
#include <cstdarg>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_SHT31.h>
#include <SerialFlash.h>

#include "debug.h"
#include "AnalogSampling.h"

class ModuleHardware {
public:
    static constexpr uint8_t PIN_FLASH_CS = 5;
    static constexpr uint8_t PIN_MAX4466 = A1;

public:
    TwoWire bno055Wire{ &sercom1, 11, 13 };
    Adafruit_SHT31 sht31Sensor;
    Adafruit_MPL3115A2 mpl3115a2Sensor;
    Adafruit_TSL2591 tsl2591Sensor{ 2591 };
    Adafruit_BNO055 bnoSensor{ 55, BNO055_ADDRESS_A, &bno055Wire };
    SerialFlashChip serialFlash;
    AnalogSampler audioSampler;

public:
    void setup() {
        SPI.begin();
        Wire.begin();

        bno055Wire.begin();

        pinPeripheral(11, PIO_SERCOM);
        pinPeripheral(13, PIO_SERCOM);

        pinMode(A3, OUTPUT);
        pinMode(A4, OUTPUT);
        pinMode(A5, OUTPUT);

        pinMode(PIN_MAX4466, INPUT);

        audioSampler.setup();
    }

    void leds(bool on) {
        digitalWrite(A3, on ? HIGH : LOW);
        digitalWrite(A4, on ? HIGH : LOW);
        digitalWrite(A5, on ? HIGH : LOW);
    }

};

class Sensors {
private:
    ModuleHardware *hw;

public:
    Sensors(ModuleHardware &hw) : hw(&hw) {
    }

public:
    void takeReading() {
        auto started = millis();

        auto shtTemperature = hw->sht31Sensor.readTemperature();
        auto shtHumidity = hw->sht31Sensor.readHumidity();

        auto pressurePascals = hw->mpl3115a2Sensor.getPressure();
        auto altitudeMeters = hw->mpl3115a2Sensor.getAltitude();
        auto mplTempCelsius = hw->mpl3115a2Sensor.getTemperature();
        auto pressureInchesMercury = pressurePascals / 3377.0;

        auto fullLuminosity = hw->tsl2591Sensor.getFullLuminosity();
        auto ir = fullLuminosity >> 16;
        auto full = fullLuminosity & 0xFFFF;
        auto lux = hw->tsl2591Sensor.calculateLux(full, ir);

        uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
        hw->bnoSensor.getCalibration(&system, &gyro, &accel, &mag);

        sensors_event_t event;
        hw->bnoSensor.getEvent(&event);

        debugfln("sensors: %fC %f%%, %fC %fpa %f\"/Hg %fm", shtTemperature, shtHumidity, mplTempCelsius, pressurePascals, pressureInchesMercury, altitudeMeters);
        debugfln("sensors: ir(%d) full(%d) visible(%d) lux(%d)", ir, full, full - ir, lux);
        debugfln("sensors: cal(%d, %d, %d, %d) xyz(%f, %f, %f)", system, gyro, accel, mag, event.orientation.x, event.orientation.y, event.orientation.z);

        hw->audioSampler.log();
    }
};

class Check {
private:
    ModuleHardware *hw;

public:
    Check(ModuleHardware &hw) : hw(&hw) {
    }

public:
    bool sht31() {
        if (!hw->sht31Sensor.begin()) {
            debugfln("test: SHT31 FAILED");
            return false;
        }

        debugfln("test: SHT31 PASSED");
        return true;
    }

    bool mpl3115a2() {
        if (!hw->mpl3115a2Sensor.begin()) {
            debugfln("test: MPL3115A2 FAILED");
            return false;
        }

        debugfln("test: MPL3115A2 PASSED");
        return true;
    }

    bool tsl2591() {
        if (!hw->tsl2591Sensor.begin()) {
            debugfln("test: TSL25911FN FAILED");
            return false;
        }

        debugfln("test: TSL25911FN PASSED");
        return true;
    }

    bool bno055() {
        if (!hw->bnoSensor.begin()) {
            debugfln("test: BNO055 FAILED");
            return false;
        }

        hw->bnoSensor.setExtCrystalUse(true);

        debugfln("test: BNO055 PASSED");
        return true;
    }

    void max4466() {
    }

    bool flashMemory() {
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

    bool check() {
        auto failures = false;
        if (!flashMemory()) {
            failures = true;
        }
        if (!bno055()) {
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

    void failed() {
        while (true) {
            hw->leds(false);
            delay(100);
            hw->leds(true);
            delay(100);
        }
    }

};

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(100);
    }

    debugfln("test: Setup");

    ModuleHardware hw;
    hw.setup();

    debugfln("test: Begin");

    Check check(hw);
    if (!check.check()) {
        check.failed();
    }

    debugfln("test: Done");

    Sensors sensors(hw);

    while (true) {
        sensors.takeReading();
    }

    delay(100);
}

void loop() {

}
