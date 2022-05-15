
project(libwjson)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_BUILD_TYPE Debug)

# add libwjson directory to include path
include_directories(AFTER lib)

# build executables
add_executable(wjsonformat cli-utils/wjsonformat/main.cpp)
add_executable(wjsoncompact cli-utils/wjsoncompact/main.cpp)
