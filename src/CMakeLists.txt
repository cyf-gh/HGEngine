set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required (VERSION 3.6)
project (HoneyGame)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH "C:/Program Files/CMake/modules")

set(SDL2_PATH "L:/SDL2-devel-2.0.12-mingw/SDL2-2.0.12/x86_64-w64-mingw32")
find_package(SDL2 REQUIRED)

aux_source_directory(. DIR_SRCS)

add_executable(hg ${DIR_SRCS} Core/Window.cc Core/Window.h Core/Init.cc Core/Init.h Core/Screen.cc Core/Screen.h Core/Renderer.cc Core/Renderer.h)
include_directories(${SDL2_INCLUDE_DIR})

target_link_libraries(hg ${SDL2_LIBRARY})
