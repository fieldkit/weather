set(SOURCE_FILES
  test.cpp
)

set(LIBRARIES
  SPI
  Wire
)

include_directories(./)

arduino(fk-weather-test "${SOURCE_FILES}" "${LIBRARIES}")
