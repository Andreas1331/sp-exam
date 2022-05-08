cmake_minimum_required(VERSION 3.21)
project(project)

set(CMAKE_CXX_STANDARD 20)

add_executable(project main.cpp include/json.hpp include/meta.hpp include/ticker.hpp include/candlestick.hpp src/candlestick.cpp include/tradestrategy.hpp include/assignmentstrategy.hpp)

include_directories(include)
include_directories(src)