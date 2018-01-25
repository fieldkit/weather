#include "FlashStorage.h"
#include "debug.h"

namespace fk {

FlashStorage::FlashStorage(SerialFlashChip &serialFlash) : serialFlash(&serialFlash) {
}

static uint32_t crc16_table[16] = {
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
    0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
};

static uint16_t crc16_update(uint16_t start, uint8_t *p, size_t n) {
    uint16_t crc = start;
    uint16_t r;

    while (n-- > 0) {
        /* compute checksum of lower four bits of *p */
        r = crc16_table[crc & 0xF];
        crc = (crc >> 4) & 0x0FFF;
        crc = crc ^ r ^ crc16_table[*p & 0xF];

        /* now compute checksum of upper four bits of *p */
        r = crc16_table[crc & 0xF];
        crc = (crc >> 4) & 0x0FFF;
        crc = crc ^ r ^ crc16_table[(*p >> 4) & 0xF];

        p++;
    }

    return crc;
}

size_t FlashStorage::write(uint8_t recordId, void *ptr, size_t size) {
    uint8_t buffer[size + sizeof(uint16_t)];

    uint16_t oldHash{ 0 };
    serialFlash->read(0, (uint8_t *)&oldHash, sizeof(uint16_t));

    auto newHash = crc16_update(0, (uint8_t *)ptr, size);
    if (newHash == oldHash) {
        return size;
    }

    memcpy(buffer, &newHash, sizeof(uint16_t));
    memcpy(buffer + sizeof(uint16_t), ptr, size);

    debugfpln("Flash", "Writing (%d)...", newHash);
    serialFlash->eraseBlock(0);
    serialFlash->write(0, buffer, sizeof(buffer));
    debugfpln("Flash", "Done (%d)", newHash);

    return size;
}

size_t FlashStorage::read(uint8_t recordId, void *ptr, size_t size) {
    uint8_t buffer[size + sizeof(uint16_t)];
    uint16_t hash{ 0 };

    serialFlash->read(0, buffer, sizeof(buffer));

    memcpy(&hash, buffer, sizeof(uint16_t));

    auto expected = crc16_update(0, buffer + sizeof(uint16_t), size);
    if (expected != hash) {
        debugfpln("Flash", "Hash mismatch (%d != %d)", expected, hash);
        return 0;
    }

    memcpy(ptr, buffer + sizeof(uint16_t), size);
    return size;
}

}
