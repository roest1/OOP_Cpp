cmake_minimum_required(VERSION 3.19)
project(fall2023-csc3380-assignment0)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
include(CTest)

add_executable(assignment0 code/assignment0.cpp)
target_compile_definitions(assignment0 PRIVATE CATCH_CONFIG_MAIN)
add_test(NAME assignment0 COMMAND assignment0)
