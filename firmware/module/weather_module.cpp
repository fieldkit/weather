#include "weather_module.h"
#include "board_definition.h"

namespace fk {

void TakeWeatherReadings::task() {
    weatherServices().weatherReadings->begin(services().readings);

    weatherServices().weatherReadings->enqueued();

    while (simple_task_run(*weatherServices().weatherReadings)) {
        services().alive();
    }

    transit<ModuleIdle>();
}


WeatherModule::WeatherModule(ModuleInfo &info) :
    Module(bus, info, { FK_WEATHER_PIN_FLASH_CS }), meters_(*moduleServices().watchdog, flashState()) {

    WeatherModuleState::weatherServices(weatherServices_);
}

void WeatherModule::tick() {
    meters_.tick();
    Module::tick();
}

}
