cmake_minimum_required(VERSION 3.4.0)

project(engine)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

#set(ANDROID_ABI "arm64-v8a")
#set(ANDROID_PLATFORM "android-24")
#include("/opt/android-sdk/ndk/21.0.6113669/build/cmake/android.toolchain.cmake")

add_library(engine STATIC
        engine.cpp
        engine.hpp
        common/module.hpp
        common/object.hpp
        window/window.cpp
        window/window.hpp
        graphics/shader.cpp
        graphics/shader.hpp
        graphics/graphics.cpp
        graphics/graphics.hpp
        graphics/texture.cpp
        graphics/texture.hpp
        image/imagedata.cpp
        image/imagedata.hpp
        system/event/event.cpp
        system/event/event.hpp
        lodepng/lodepng.cpp
        lodepng/lodepng.hpp
)

target_link_libraries(engine INTERFACE EGL GLESv3 android)
#set_target_properties(engine PROPERTIES CXX_VISIBILITY_PRESET hidden)
#set_target_properties(engine PROPERTIES VISIBILITY_INLINES_HIDDEN 1)