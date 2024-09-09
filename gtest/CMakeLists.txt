cmake_minimum_required(VERSION 3.25)
project(gtest-sandbox)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(gtest-sandbox main.cpp)

target_link_libraries(gtest-sandbox
     PRIVATE
        GTest::gtest_main)
