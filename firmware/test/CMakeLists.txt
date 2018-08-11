set(SOURCE_FILES
  main.cpp
  debug.cpp
  Check.cpp
  ../common/AmbientSensors.cpp
  ../common/WeatherMeters.cpp
  ../common/ModuleHardware.cpp
)

set(LIBRARIES
  SPI
  Wire
)

include_directories(./)

arduino(fk-weather-test "${SOURCE_FILES}" "${LIBRARIES}")

target_compile_options(fk-weather-test PUBLIC -Wall -Werror -I${CMAKE_CURRENT_SOURCE_DIR}/../common)

add_definitions(-DPHYLUM_ENABLE_SERIAL_FLASH)
target_compile_options(arduino_zero_phylum PUBLIC -DPHYLUM_ENABLE_SERIAL_FLASH)
