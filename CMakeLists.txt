cmake_minimum_required(VERSION 3.29)
project(game_of_life)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)

add_executable(game_of_life main.cpp)

target_link_libraries(game_of_life raylib)
