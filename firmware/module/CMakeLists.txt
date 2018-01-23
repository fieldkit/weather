set(SOURCE_FILES
  main.cpp
  readings.cpp
  ../common/WeatherMeters.cpp
)

set(LIBRARIES
  Wire
  SPI
)

arduino(fk-weather-module "${SOURCE_FILES}" "${LIBRARIES}")

target_compile_options(fk-weather-module PUBLIC -Wall -Werror -I${CMAKE_CURRENT_SOURCE_DIR}/../common)
