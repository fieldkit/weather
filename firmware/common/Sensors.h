#ifndef FK_SENSORS_H_INCLUDED
#define FK_SENSORS_H_INCLUDED

#include "debug.h"
#include "ModuleHardware.h"

namespace fk {

class Sensors {
private:
    ModuleHardware *hw;

public:
    Sensors(ModuleHardware &hw) : hw(&hw) {
    }

public:
    void takeReading() {
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

        debugfln("sensors: %fC %f%%, %fC %fpa %f\"/Hg %fm", shtTemperature, shtHumidity, mplTempCelsius, pressurePascals, pressureInchesMercury, altitudeMeters);
        debugfln("sensors: ir(%lu) full(%lu) visible(%lu) lux(%f)", ir, full, full - ir, lux);
    }
};

}

#endif
