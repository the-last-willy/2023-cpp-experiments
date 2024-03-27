cmake_minimum_required(VERSION 3.25)
project(dynamic-std)

find_package(Catch2 REQUIRED)

add_library(dynamic-std)
target_sources(dynamic-std
        PUBLIC
        dvector.hpp
        raw_vector.hpp
        stateful_fn.hpp
        PRIVATE
        dvector.cpp
        raw_vector.cpp
)

add_executable(dynamic-std-sandbox sandbox.cpp)
target_link_libraries(dynamic-std-sandbox
        dynamic-std
)

add_executable(dynamic-std-tests dvector.tests.cpp)
target_sources(dynamic-std-tests
        PRIVATE
        raw_vector.tests.cpp
        stateful_fn.test.cpp
)
target_link_libraries(dynamic-std-tests
        Catch2::Catch2WithMain
        dynamic-std
)
