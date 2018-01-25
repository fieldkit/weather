#ifndef FK_FLASH_STORAGE_H_INCLUDED
#define FK_FLASH_STORAGE_H_INCLUDED

#include <Arduino.h>
#include <SerialFlash.h>

namespace fk {

class FlashStorage {
private:
    SerialFlashChip *serialFlash;
    bool createNew{ true };

public:
    FlashStorage(SerialFlashChip &serialFlash);

public:

    size_t write(uint8_t recordId, void *ptr, size_t size);
    size_t read(uint8_t recordId, void *ptr, size_t size);
};

}

#endif
