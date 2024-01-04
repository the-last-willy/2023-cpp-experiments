cmake_minimum_required(VERSION 3.25)
project(rival-parameter)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(rival-parameter main.cpp)
