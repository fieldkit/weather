set(SOURCE_FILES
  main.cpp
  readings.cpp
)

set(LIBRARIES
  Wire
  SPI
)

arduino(fk-weather-module "${SOURCE_FILES}" "${LIBRARIES}")
