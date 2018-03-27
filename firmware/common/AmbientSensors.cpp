#include "AmbientSensors.h"

namespace fk {

AmbientSensors::AmbientSensors(ModuleHardware &hw) : hw(&hw) {
}

void AmbientSensors::takeReading() {
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

    debugfpln("Sensors", "sensors: %fC %f%%, %fC %fpa %f\"/Hg %fm", shtTemperature, shtHumidity, mplTempCelsius, pressurePascals, pressureInchesMercury, altitudeMeters);
    debugfpln("Sensors", "sensors: ir(%lu) full(%lu) visible(%lu) lux(%f)", ir, full, full - ir, lux);
}

}
