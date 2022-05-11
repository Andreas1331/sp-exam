cmake_minimum_required(VERSION 3.21)
project(project)

set(CMAKE_CXX_STANDARD 20)

add_executable(project main.cpp include/json.hpp include/meta.hpp include/ticker.hpp include/candlestick.hpp src/candlestick.cpp include/tradestrategy.hpp include/assignmentstrategy.hpp include/tradestamp.hpp include/holdingstrategy.hpp include/benchmark.hpp src/benchmark.cpp)

include_directories(include)
include_directories(src)

add_library(doctest OBJECT src/doctest.cpp)


add_executable(unittests src/unittests.cpp include/candlestick.hpp src/candlestick.cpp)
target_link_libraries(unittests PRIVATE doctest)

add_test(NAME unit_tests COMMAND unittests)
add_test(NAME plot_tests COMMAND python ${CMAKE_SOURCE_DIR}/py/plotter.py)

enable_testing()
