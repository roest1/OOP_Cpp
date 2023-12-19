cmake_minimum_required(VERSION 3.19)
project(fall2023-csc3380-assignment3)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
include(CTest)

add_executable(container_warmup code/container_warmup.cpp)
target_compile_definitions(container_warmup PRIVATE CATCH_CONFIG_MAIN)
add_test(NAME container_warmup COMMAND container_warmup WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_executable(knights_tour code/knights_tour.cpp)
target_compile_definitions(knights_tour PRIVATE CATCH_CONFIG_MAIN)
add_test(NAME knights_tour COMMAND knights_tour WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_executable(eight_queens code/eight_queens.cpp)
target_compile_definitions(eight_queens PRIVATE CATCH_CONFIG_MAIN)
add_test(NAME eight_queens COMMAND eight_queens WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_executable(maze code/maze.cpp)
target_compile_definitions(maze PRIVATE CATCH_CONFIG_MAIN)
add_test(NAME maze COMMAND maze WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_executable(maze_generator code/maze_generator.cpp)
