include(Arduino)

enable_arduino_toolchain()

file(GLOB sources *.cpp ../common/*.cpp)

find_package(FkModule)

find_package(Adafruit_Sensor)
find_package(Adafruit_MPL3115A2)
find_package(Adafruit_SHT31)
find_package(Adafruit_TSL2561)
find_package(Adafruit_TSL2591)

add_arduino_library(fk-weather-standard "${sources}")

fk_configure_module(fk-weather-standard)

target_link_libraries(fk-weather-standard Adafruit_Sensor)
target_link_libraries(fk-weather-standard Adafruit_MPL3115A2_Library)
target_link_libraries(fk-weather-standard Adafruit_SHT31)
target_link_libraries(fk-weather-standard Adafruit_TSL2561)
target_link_libraries(fk-weather-standard Adafruit_TSL2591_Library)

target_compile_options(fk-weather-standard PRIVATE -Wall -Werror)
target_include_directories(fk-weather-standard PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../common")

add_arduino_firmware(fk-weather-standard)
