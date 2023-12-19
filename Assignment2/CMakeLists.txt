cmake_minimum_required(VERSION 3.27)
project(fall2023-csc3380-assignment2)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
include(CTest)

add_executable(warmup code/warmup.cpp)
target_compile_definitions(
    warmup PRIVATE 
    CATCH_CONFIG_MAIN 
    CATCH_CONFIG_ENABLE_BENCHMARKING)
add_test(NAME warmup COMMAND warmup WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_executable(balanced code/balanced.cpp)
target_compile_definitions(
    balanced PRIVATE 
    CATCH_CONFIG_MAIN 
    CATCH_CONFIG_ENABLE_BENCHMARKING)
add_test(NAME balanced COMMAND balanced WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_executable(sierpinski code/sierpinski.cpp)
target_compile_definitions(
    sierpinski PRIVATE 
    CATCH_CONFIG_MAIN 
    CATCH_CONFIG_ENABLE_BENCHMARKING)
add_test(NAME sierpinski COMMAND sierpinski WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_executable(binary_merge code/binary_merge.cpp)
target_compile_definitions(
    binary_merge PRIVATE 
    CATCH_CONFIG_MAIN 
    CATCH_CONFIG_ENABLE_BENCHMARKING)
add_test(NAME binary_merge COMMAND binary_merge WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
