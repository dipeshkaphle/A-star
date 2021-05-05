cmake_minimum_required(VERSION "3.16.3")

project("A_star")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -fsanitize=address -O3")

set(CMAKE_CXX_STANDARD 17)

add_executable(all_A_stars all_A_stars.cpp)

add_executable(demo demo.cpp)

add_executable(profile_operations profile_operations.cpp)
