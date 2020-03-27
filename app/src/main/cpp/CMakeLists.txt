cmake_minimum_required(VERSION 3.4.1)

project(battleship)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(engine)

add_subdirectory(engine)
add_library(battleship SHARED engine/engine.cpp main.cpp)

target_link_libraries(battleship engine)