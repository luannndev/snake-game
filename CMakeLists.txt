cmake_minimum_required(VERSION 3.29)
project(snake_game CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(snake_game main.cpp)
target_link_libraries(snake_game ${CURSES_LIBRARIES})