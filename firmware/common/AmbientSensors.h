#ifndef FK_AMBIENT_SENSORS_H_INCLUDED
#define FK_AMBIENT_SENSORS_H_INCLUDED

#include "debug.h"
#include "ModuleHardware.h"

namespace fk {

class AmbientSensors {
private:
    WeatherHardware *hw;

public:
    AmbientSensors(WeatherHardware &hw);

public:
    void takeReading();

};

}

#endif
