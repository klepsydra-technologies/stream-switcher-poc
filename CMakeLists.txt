
cmake_minimum_required(VERSION 2.8.11)

set(PROJ_MAIN_NAME "stream_switcher")
project(${PROJ_MAIN_NAME})

set(
  ${PROJ_MAIN_NAME}_LIB_TYPE
  "SHARED"
  CACHE STRING "Choose STATIC or SHARED to generate libraries" FORCE
)

# Configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if (DEFINED CMAKE_BUILD_TYPE AND CMAKE_VERSION VERSION_GREATER "2.8")
  set_property(
    CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES}
  )
endif ()

# Install binaries
if (DEFINED MAIN_INSTALL_PREFIX)
else ()
  set(MAIN_INSTALL_PREFIX $ENV{HOME}/install)
endif ()
message(STATUS "Main install path: " ${MAIN_INSTALL_PREFIX})

if (DEFINED THIRDPARTIES_PATH)
else ()
  set(THIRDPARTIES_PATH $ENV{HOME}/thirdparties)
endif ()
message(STATUS "Thirdparties install path: " ${THIRDPARTIES_PATH})

if (DEFINED GTEST_PATH)
else ()
  set(GTEST_PATH ${THIRDPARTIES_PATH}/googletest)
endif ()
message(STATUS "Google test install path: " ${GTEST_PATH})

# Paths
set(CMAKE_MODULE_PATH ${CMAKE_TEMPLATE_DIR}/cmake ${CMAKE_SOURCE_DIR}/cmake)
set(
  LIBRARY_OUTPUT_PATH
  ${PROJECT_BINARY_DIR}/bin
  CACHE PATH "Library output" FORCE
)
set(
  EXECUTABLE_OUTPUT_PATH
  ${PROJECT_BINARY_DIR}/bin
  CACHE PATH "Executable output" FORCE
)
set(
  ${PROJ_MAIN_NAME}_PATH_MAIN
  ${CMAKE_SOURCE_DIR}
  CACHE PATH "This directory contains initial Path"
)
set(
  ${PROJ_MAIN_NAME}_PATH_LIBS
  ${PROJECT_BINARY_DIR}/gen
  CACHE PATH "This directory contains all libs"
)
set(
  ${PROJ_MAIN_NAME}_PATH_EXE
  ${PROJECT_BINARY_DIR}/gen
  CACHE PATH "This directory contains executables"
)
set(
  ${PROJ_MAIN_NAME}_PATH_DATA
  ${${PROJ_MAIN_NAME}_PATH_MAIN}/extras/data/
  CACHE PATH "This directory contains all DATA and RESOURCES"
)
set(
  TEST_DATA
  ${CMAKE_SOURCE_DIR}/tests/data
  CACHE PATH "Path to data for tests"
)

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-std=gnu++11 HAS_CXX11)
if (HAS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif ()

find_package(Klepsydra REQUIRED)

add_definitions(-std=c++11)
enable_testing()

find_package(OpenCV REQUIRED)
if (OpenCV_DIR STREQUAL NOT_FOUND)
  message(FATAL_ERROR "Install OpenCV")
endif ()

# Project Logic
# ------------------------------------------------------------------------------
if (NOT TARGET_PLATFORM STREQUAL TARGET_NONE)
  add_subdirectory(modules)
  add_subdirectory(tests)
else ()
  message(
    FATAL_ERROR " TARGET no autodetected. Please, select a TARGET manually. "
  )
endif ()
