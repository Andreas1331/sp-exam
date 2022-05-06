cmake_minimum_required(VERSION 3.21)
project(project)

set(CMAKE_CXX_STANDARD 20)

add_executable(project main.cpp include/json.hpp include/meta.hpp include/ticker.hpp)
