cmake_minimum_required(VERSION 3.25)
project(rival-module-builder)

find_package(inja CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_library(rival-module-builder INTERFACE rival-module-builder.h)
target_compile_definitions(rival-module-builder INTERFACE OUTPUTS_FOLDERPATH="${CMAKE_CURRENT_SOURCE_DIR}/outputs/")
target_compile_definitions(rival-module-builder INTERFACE TEMPLATES_FOLDERPATH="${CMAKE_CURRENT_SOURCE_DIR}/templates/")
target_include_directories(rival-module-builder INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rival-module-builder INTERFACE pantor::inja)

add_executable(rival-module-builder-example main.cpp)
target_link_libraries(rival-module-builder-example PRIVATE rival-module-builder)
