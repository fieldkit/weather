set(SOURCE_FILES
  main.cpp
  WeatherMeters.cpp
)

set(LIBRARIES
  SPI
  Wire
)

include_directories(./)

arduino(fk-weather-test "${SOURCE_FILES}" "${LIBRARIES}")
