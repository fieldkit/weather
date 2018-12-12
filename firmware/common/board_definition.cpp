#include "board_definition.h"

namespace fk {

Board board{
    {
        FK_WEATHER_PIN_PERIPH_ENABLE,
        FK_WEATHER_PIN_FLASH_CS,
        {
            FK_WEATHER_PIN_FLASH_CS,
            0,
            0,
            0,
        },
        {
            FK_WEATHER_PIN_PERIPH_ENABLE,
            0,
            0,
            0,
        }
    }
};

}
