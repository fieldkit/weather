#ifndef FK_BOARD_DEFINITION_H_INCLUDED
#define FK_BOARD_DEFINITION_H_INCLUDED

#include "board.h"

namespace fk {

constexpr uint8_t FK_WEATHER_PIN_FLASH_CS = 5;
constexpr uint8_t FK_WEATHER_PIN_WEATHER_ENABLE = 7;
constexpr uint8_t FK_WEATHER_PIN_PERIPHERALS_ENABLE = 8;

extern Board board;

}

#endif
