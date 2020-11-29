find_package( SDL2 REQUIRED )
include_directories( ${SDL2_INCLUDE_DIR} )

aux_source_directory(. DIR_CORE_SRCS)
add_library (HGCore ${DIR_CORE_SRCS})