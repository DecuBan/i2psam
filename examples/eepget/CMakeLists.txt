add_executable(eepget ${CMAKE_CURRENT_SOURCE_DIR}/src/eepget.cpp)

target_link_libraries(eepget PRIVATE i2psam)
