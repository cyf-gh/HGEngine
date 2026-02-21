cmake_minimum_required (VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Project name
project (HoneyGame VERSION 1.0.0 LANGUAGES CXX)

# Find SDL2 from third_party
set(SDL2_ROOT "${CMAKE_SOURCE_DIR}/../third_party/SDL2" CACHE PATH "SDL2 root directory")
set(SDL2_IMAGE_ROOT "${CMAKE_SOURCE_DIR}/../third_party/SDL2_image" CACHE PATH "SDL2_image root directory")
set(SDL2_TTF_ROOT "${CMAKE_SOURCE_DIR}/../third_party/SDL2_ttf" CACHE PATH "SDL2_ttf root directory")

# Find SDL2
find_package(SDL2 REQUIRED PATHS "${SDL2_ROOT}" NO_DEFAULT_PATH)
find_package(SDL2_image REQUIRED PATHS "${SDL2_IMAGE_ROOT}" NO_DEFAULT_PATH)
find_package(SDL2_ttf REQUIRED PATHS "${SDL2_TTF_ROOT}" NO_DEFAULT_PATH)

# Find RapidJSON
find_path(RAPIDJSON_INCLUDE_DIR "rapidjson/document.h" 
    PATHS "${CMAKE_SOURCE_DIR}/../third_party/rapidjson/include"
    NO_DEFAULT_PATH)

# Find nameof
find_path(NAMEOF_INCLUDE_DIR "nameof.hpp"
    PATHS "${CMAKE_SOURCE_DIR}/../third_party/nameof/include"
    NO_DEFAULT_PATH)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2IMAGE_INCLUDE_DIR}
    ${SDL2TTF_INCLUDE_DIR}
    ${RAPIDJSON_INCLUDE_DIR}
    ${NAMEOF_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine
    ${CMAKE_CURRENT_SOURCE_DIR}/EngineImpl
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
)

# Find all source files
aux_source_directory(. DIR_MAIN_SRCS)
aux_source_directory(Core DIR_CORE_SRCS)
aux_source_directory(EngineImpl DIR_ENGINEIMPL_SRCS)

# Add subdirectories
add_subdirectory(Core)
add_subdirectory(EngineImpl)

# Create executable
add_executable(hg ${DIR_MAIN_SRCS})

# Link libraries
target_link_libraries(hg PRIVATE 
    HGCore 
    EngineImpl
    ${SDL2_LIBRARY}
    ${SDL2IMAGE_LIBRARY}
    ${SDL2TTF_LIBRARY}
)

# Copy DLLs to output directory
if(WIN32)
    add_custom_command(TARGET hg POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_LIBRARY_DIR}/SDL2.dll
            $<TARGET_FILE_DIR:hg>
    )
endif()

# Enable testing
enable_testing()

# Add test executable
add_executable(hg_test 
    Core/test/main.cpp
)

target_link_libraries(hg_test PRIVATE
    HGCore
    EngineImpl
    ${SDL2_LIBRARY}
    ${SDL2IMAGE_LIBRARY}
    ${SDL2TTF_LIBRARY}
)

add_test(NAME HGEngineTests COMMAND hg_test)

# Print configuration summary
message(STATUS "=== HGEngine Configuration ===")
message(STATUS "SDL2: ${SDL2_LIBRARY}")
message(STATUS "SDL2_image: ${SDL2IMAGE_LIBRARY}")
message(STATUS "SDL2_ttf: ${SDL2TTF_LIBRARY}")
message(STATUS "RapidJSON: ${RAPIDJSON_INCLUDE_DIR}")
message(STATUS "nameof: ${NAMEOF_INCLUDE_DIR}")
message(STATUS "===========================")
