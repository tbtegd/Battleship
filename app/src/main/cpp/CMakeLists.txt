cmake_minimum_required(VERSION 3.4.1)

add_library(battleship SHARED
        native.cpp
        engine.h
        engine.cpp)
target_link_libraries(battleship EGL GLESv2 android log)