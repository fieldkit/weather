#include "board_definition.h"

namespace fk {

Board board{
    {
        FK_WEATHER_PIN_PERIPHERALS_ENABLE,
        FK_WEATHER_PIN_FLASH_CS,
        {
            FK_WEATHER_PIN_FLASH_CS,
            0,
            0,
            0,
        },
        {
            FK_WEATHER_PIN_PERIPHERALS_ENABLE,
            FK_WEATHER_PIN_WEATHER_ENABLE,
            0,
            0,
        }
    }
};

}
