#ifndef FK_AMBIENT_SENSORS_H_INCLUDED
#define FK_AMBIENT_SENSORS_H_INCLUDED

#include "debug.h"
#include "ModuleHardware.h"

namespace fk {

class AmbientSensors {
private:
    ModuleHardware *hw;

public:
    AmbientSensors(ModuleHardware &hw);

public:
    void takeReading();

};

}

#endif
