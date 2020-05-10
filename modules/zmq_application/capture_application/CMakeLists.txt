
cmake_minimum_required(VERSION 2.8.3)
set(PROJ_NAME capture_application)
project(${PROJ_NAME})

add_definitions(-std=c++11)

find_package(Klepsydra REQUIRED)
find_package(cppzmq REQUIRED)

set(MAIN_INSTALL_PREFIX $ENV{HOME}/install)
set(THIRDPARTIES_PATH $ENV{HOME}/thirdparties)
set(CPT_LIBRARY capture_svc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/zmq_application/common)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/services/capture_svc/include)
include_directories(${KLEPSYDRA_INCLUDE_DIRS})
include_directories()

file(GLOB ${PROJ_NAME}_SRC "src/*.cpp")
file(GLOB ${PROJ_NAME}_HEADERS
    "include/*.h"
)

link_directories(${${PROJ_MAIN_NAME}_PATH_LIBS})

add_executable(${PROJ_NAME} ${${PROJ_NAME}_HEADERS} ${${PROJ_NAME}_SRC})

target_link_libraries(
    ${PROJ_NAME}
    gtest_main
    atomic
    yaml-cpp
    kpsr_core_yaml 
    kpsr_vision_ocv 
    ${KLEPSYDRA_CORE_LIBRARIES}
    ${KLEPSYDRA_ROBOTICS_LIBRARIES}
    libzmq
    ${CPT_LIBRARY}
    ${OpenCV_LIBS}
)

install(
)
