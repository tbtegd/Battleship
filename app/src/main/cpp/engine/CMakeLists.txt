cmake_minimum_required(VERSION 3.4.1)

project(engine)

include_directories(.)

set(CMAKE_CXX_STANDARD 17)

add_library(engine STATIC
    module.hpp
    event/event.cpp
    event/event.hpp
    graphics/graphics.cpp
    graphics/graphics.hpp
    window/window.cpp
    window/window.hpp
    window/android/window.cpp
    window/android/window.hpp)

target_link_libraries(engine EGL GLESv2 GLESv3 android log)