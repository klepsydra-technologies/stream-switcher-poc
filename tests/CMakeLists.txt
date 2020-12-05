find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

message(STATUS "gtest_path: " ${GTEST_PATH})
message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
# Add googletest directly to our build. This defines the gtest and gtest_main
# targets.
add_subdirectory(${GTEST_PATH} ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

include_directories("${source_dir}/include")

add_subdirectory(services_test)