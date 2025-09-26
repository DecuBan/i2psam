cmake_minimum_required(VERSION 3.35...4.1)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed")
endif()

project(
    i2psam
    DESCRIPTION "A C++ library for the SAM v3 API"
    LANGUAGES C CXX
)

option(BUILD_STATIC_LIB "Build static library." OFF)
option(BUILD_EXAMPLES "Build examples." OFF)

message(STATUS "C compiler: " ${CMAKE_C_COMPILER_ID})
message(STATUS "CXX compiler: " ${CMAKE_CXX_COMPILER_ID})
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

set(I2PSAM_MAJOR_VERSION 3)
set(I2PSAM_MINOR_VERSION 0)
set(I2PSAM_PATCH_VERSION 5)
set(I2PSAM_VERSION ${I2PSAM_MAJOR_VERSION}.${I2PSAM_MINOR_VERSION}.${I2PSAM_PATCH_VERSION})
set(I2PSAM_VERSION_STRING "${I2PSAM_VERSION}")

set(CMAKE_INSTALL_PREFIX /usr/)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

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

set(LIB_HEADERS
    include/i2psam/compat.h
    include/i2psam/i2psam-c.h
    include/i2psam/i2psam.h
    include/i2psam/utility.h
)
set(LIB_SOURCES
    src/i2psam.cpp
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${LIB_HEADERS}
    ${LIB_SOURCES}
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

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/${I2PSAM_VERSION}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/${I2PSAM_VERSION}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/exports/${PROJECT_NAME}/exports.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${I2PSAM_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR
)

install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
endif()
