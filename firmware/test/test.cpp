#include <Wire.h>
#include <SPI.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_MPL3115A2.h>

class Adafruit_SHT31 {
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
    Adafruit_SHT31(uint8_t address) {
        this->address = address;
    }

    bool begin() {
        uint8_t value = read8(0x12);
        return value == 0x50;
    }
};

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
       uint8_t value = read8(0x12);
       Serial.println(value);
       return value == 0x50;
   }
};

class Check {
public:
    void setup() {
        SPI.begin();
        Wire.begin();
    }

    void sht31() {
        Adafruit_SHT31 sensor(0x44);
        if (!sensor.begin()) {
            Serial.println("test: SHT31 FAILED");
        }
        else {
            Serial.println("test: SHT31 PASSED");
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
        Adafruit_BNO055 bno(55, BNO055_ADDRESS_A);

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

};

void setup() {
    Serial.begin(115200);

    Check check;
    check.setup();

    pinMode(13, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);

    digitalWrite(13, HIGH);
    digitalWrite(A3, HIGH);
    digitalWrite(A4, HIGH);
    digitalWrite(A5, HIGH);

    while (!Serial) {
        delay(100);
    }

    digitalWrite(13, LOW);
    digitalWrite(A3, LOW);
    digitalWrite(A4, LOW);
    digitalWrite(A5, LOW);

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
        check.mpl3115a2();
        check.tsl2591();
        check.sht31();
        check.bno055();
        delay(2000);
    }
    // check.max4466();

    Serial.println("test: Done");

    delay(100);
}

void loop() {

}
