#ifndef FK_CHECK_H_INCLUDED
#define FK_CHECK_H_INCLUDED

#include <ModuleHardware.h>

namespace fk {

class Check {
private:
    ModuleHardware *hw;

public:
    Check(ModuleHardware &hw);

public:
    bool sht31();
    bool mpl3115a2();
    bool tsl2591();
    bool flashMemory();
    bool check();
    void failed();

};

}

#endif
