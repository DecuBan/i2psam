cmake_minimum_required(VERSION 3.10...4.1)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed")
endif()

project(
    i2psam
    DESCRIPTION "A C++ library for the SAM v3 API"
    LANGUAGES C CXX
)

option(BUILD_STATIC_LIB "Build static library." OFF)
option(BUILD_EXAMPLES "Build examples." ON)

message(STATUS "C compiler: " ${CMAKE_C_COMPILER_ID})
message(STATUS "CXX compiler: " ${CMAKE_CXX_COMPILER_ID})
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

set(I2PSAM_MAJOR_VERSION 3)
set(I2PSAM_MINOR_VERSION 0)
set(I2PSAM_PATCH_VERSION 5)
set(I2PSAM_VERSION ${I2PSAM_MAJOR_VERSION}.${I2PSAM_MINOR_VERSION}.${I2PSAM_PATCH_VERSION})
set(I2PSAM_VERSION_STRING "${I2PSAM_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

file(CREATE_LINK "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json" SYMBOLIC)

if (NOT BUILD_STATIC_LIB)
    add_library(${PROJECT_NAME} SHARED)
else ()
    add_library(${PROJECT_NAME} STATIC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES "${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif (NOT BUILD_STATIC_LIB)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif (BUILD_EXAMPLES)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/exports/${PROJECT_NAME}/exports.h)

if (CMAKE_BUILD_TYPE)
    string(TOLOWER ${CMAKE_BUILD_TYPE} _type)
    if (_type STREQUAL release)
        set(COMPILATION_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -std=c++")
    elseif (_type STREQUAL debug)
        set(COMPILATION_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -std=c++")
    else ()
        set(COMPILATION_FLAGS "-std=c++")
    endif ()
else (CMAKE_BUILD_TYPE)
    set(COMPILATION_FLAGS "-std=c++")
endif (CMAKE_BUILD_TYPE)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/i2psam.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/exports>
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    VERSION ${I2PSAM_VERSION}
    SOVERSION ${I2PSAM_MAJOR_VERSION}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
# set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME i2psam)
