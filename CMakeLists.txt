cmake_minimum_required(VERSION 3.8)
project(ncage)

set(FREEGLUT_LIBRARIES libopengl32.a libglu32.a libglew32.dll.a libglfw3.a -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11 -m64")

set(SOURCES
        "ncage/main.cpp"
        )

include_directories(src)
include_directories(external)
