cmake_minimum_required(VERSION 3.0.0)
project(pearson VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/include)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

add_executable(pearson src/pearson.cpp)

