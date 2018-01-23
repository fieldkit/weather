set(SOURCE_FILES
  main.cpp
  debug.cpp
  Check.cpp
  ../common/WeatherMeters.cpp
)

set(LIBRARIES
  SPI
  Wire
)

include_directories(./)

arduino(fk-weather-test "${SOURCE_FILES}" "${LIBRARIES}")

target_compile_options(fk-weather-test PUBLIC -Wall -Werror -I${CMAKE_CURRENT_SOURCE_DIR}/../common)
