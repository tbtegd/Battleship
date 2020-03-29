cmake_minimum_required(VERSION 3.4.0)

project(battleship)

set(CMAKE_CXX_STANDARD 17)

include_directories("." "engine")

add_subdirectory(engine)

#set(ANDROID_ABI "arm64-v8a")
#set(ANDROID_PLATFORM "android-24")
#include("/opt/android-sdk/ndk/21.0.6113669/build/cmake/android.toolchain.cmake")

add_library(battleship SHARED
        math.hpp
        main.cpp)

target_link_libraries(battleship engine)
#set_target_properties(battleship PROPERTIES CXX_VISIBILITY_PRESET hidden)
#set_target_properties(battleship PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
#target_link_options(battleship PUBLIC
#    -Wl,--strip
#    -Wl,--exclude-libs,ALL
#)