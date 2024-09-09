cmake_minimum_required(VERSION 3.25)
project(composite)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

add_executable(composite Composite.cpp Composite.tests.cpp)

target_link_libraries(composite
        PRIVATE
        GTest::gtest_main)
