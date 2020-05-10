set(PROJ_NAME stream_switcher_svc_test)
project(${PROJ_NAME})

set(SSW_LIBRARY stream_switcher_svc)

# Include directories with headers
# ---------------------------------------------------#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/services/stream_switcher_svc/include)
include_directories(${KLEPSYDRA_INCLUDE_DIRS})

# Source
# ---------------------------------------------------#
file(GLOB ${PROJ_NAME}_SRC "src/*.cpp")

# Link libraries with Project
# ---------------------------------------------------#
link_directories(${${PROJ_MAIN_NAME}_PATH_LIBS})

# Create Library
# ---------------------------------------------------#
add_executable(${PROJ_NAME} ${${PROJ_NAME}_HEADERS} ${${PROJ_NAME}_SRC})

target_link_libraries(
  ${PROJ_NAME}
  gtest_main
  atomic
  ${SSW_LIBRARY}
  ${OpenCV_LIBS}
)

add_test(NAME ${PROJ_NAME} COMMAND ${PROJ_NAME})
