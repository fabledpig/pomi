cmake_minimum_required (VERSION 2.8.11)
project (pomi)

set (CMAKE_CXX_STANDARD 14)

add_executable (pomi
  src/pomi.cpp
  src/i2cdevice.cpp
  src/gy30.cpp
  src/spidevice.cpp
  src/sensordata.cpp
  src/bme280.cpp
  BME280_driver/bme280.c)

target_link_libraries (pomi mysqlclient)
target_include_directories (pomi PUBLIC include .)
