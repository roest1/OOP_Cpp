cmake_minimum_required(VERSION 3.19)
project(fall2023-csc3380-assignment4)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
include(CTest)

add_executable(labyrinth
    code/is_path_to_freedom.cpp
    code/labyrinth.cpp
    code/maze_generator.cpp
    code/hash_code.cpp)
    
add_test(
    NAME labyrinth
    COMMAND labyrinth
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_executable(labyrinth_escape 
    code/is_path_to_freedom.cpp
    code/labyrinth_escape.cpp
    code/maze_generator.cpp
    code/hash_code.cpp)
add_test(
    NAME labyrinth_escape 
    COMMAND labyrinth_escape 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_executable(sorting_linked_lists
    code/sorting_linked_lists.cpp)
target_compile_definitions(
    sorting_linked_lists PRIVATE 
    CATCH_CONFIG_MAIN
    CATCH_CONFIG_ENABLE_BENCHMARKING)
add_test(
    NAME sorting_linked_lists 
    COMMAND sorting_linked_lists 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
