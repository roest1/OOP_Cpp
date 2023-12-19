cmake_minimum_required(VERSION 3.19)
project(fall2023-csc3380-assignment1)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
include(CTest)

add_executable(perfect_numbers code/perfect_numbers.cpp)

add_executable(benchmark code/benchmark.cpp)
target_compile_definitions(
  benchmark PRIVATE 
  CATCH_CONFIG_ENABLE_BENCHMARKING
  CATCH_CONFIG_MAIN)
add_test(NAME benchmark COMMAND benchmark WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_executable(soundex code/soundex.cpp)
target_compile_definitions(
  soundex PRIVATE 
  CATCH_CONFIG_RUNNER)
add_test(NAME soundex COMMAND soundex WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
