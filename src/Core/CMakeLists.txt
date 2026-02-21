# Core library
aux_source_directory(. DIR_CORE_SRCS)

add_library(HGCore STATIC ${DIR_CORE_SRCS})

# Include directories for Core
target_include_directories(HGCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Compile definitions
target_compile_definitions(HGCore PRIVATE
    _CRT_SECURE_NO_WARNINGS
)