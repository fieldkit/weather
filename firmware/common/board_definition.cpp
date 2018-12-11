#include "board_definition.h"

namespace fk {

Board<BoardConfig<1, 1>> board{
    {
        FK_WEATHER_PIN_PERIPH_ENABLE,
        FK_WEATHER_PIN_FLASH_CS,
        {
            FK_WEATHER_PIN_FLASH_CS,
        },
        {
            FK_WEATHER_PIN_PERIPH_ENABLE,
        }
    }
};

}
