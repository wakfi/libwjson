project(LIBWJSON_GTESTS)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_BUILD_TYPE Debug)

# add libwjson directory to include path
include_directories(AFTER ../lib)

# locate gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# create unit test executable
add_executable(wjson_core_test
               unit/core.test.cpp)
target_link_libraries(wjson_core_test ${GTEST_LIBRARIES} pthread)
