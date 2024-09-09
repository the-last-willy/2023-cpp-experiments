cmake_minimum_required(VERSION 3.25)
project(imperative-composite)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

add_executable(composite main.cpp)

target_link_libraries(composite
        PRIVATE
        GTest::gtest_main)
