# EngineImpl library
aux_source_directory(. DIR_ENGINE_IMPL_SRCS)

add_library(EngineImpl STATIC ${DIR_ENGINE_IMPL_SRCS})

# Include directories for EngineImpl
target_include_directories(EngineImpl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Editor
    ${CMAKE_CURRENT_SOURCE_DIR}/GUI
)

# Link dependencies
target_link_libraries(EngineImpl PUBLIC HGCore)

# Compile definitions
target_compile_definitions(EngineImpl PRIVATE
    _CRT_SECURE_NO_WARNINGS
    HGENGINE_PHYSICS
)

# Add imgui library path for linking
if(WIN32)
    target_link_libraries(EngineImpl PRIVATE
        "${CMAKE_SOURCE_DIR}/../msvc/Libs/imgui_msvc_d.lib"
    )
endif()

