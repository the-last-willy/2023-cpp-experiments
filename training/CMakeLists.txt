cmake_minimum_required(VERSION 3.25)
project(training)

find_package(Catch2 REQUIRED)

add_executable(text-processing tp.cpp)

add_library(vector-impl INTERFACE)
target_sources(vector-impl
        INTERFACE
        vector-impl.h
)

add_executable(vector-impl-tests vector-impl-tests.cpp)
target_link_libraries(vector-impl-tests
        Catch2::Catch2WithMain
        vector-impl
)
