cmake_minimum_required (VERSION 3.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_FLAGS "-Wall")
# set(CMAKE_CXX_FLAGS_DEBUG   "-O0" )
set(CMAKE_MODULE_PATH "C:/Program Files/CMake/modules")

project (HoneyGame)

# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)

find_package(SDL2 REQUIRED)

aux_source_directory(. DIR_MAIN_SRCS)
add_subdirectory(Core)
add_subdirectory(EngineImpl)
add_executable(hg ${DIR_MAIN_SRCS} )

include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries( hg ${SDL2_LIBRARY} )
target_link_libraries( hg HGCore )
target_link_libraries( hg EngineImpl )
