add_executable(eepget ${CMAKE_CURRENT_SOURCE_DIR}/src/eepget.cpp)

find_package(i2psam CONFIG REQUIRED)

target_link_libraries(eepget PRIVATE i2psam::i2psam)
