find_package( SDL2 REQUIRED )
include_directories( ${SDL2_INCLUDE_DIR} )

aux_source_directory(. DIR_ENGINE_IMPL_SRCS)
add_library (HGEngineImpl ${DIR_ENGINE_IMPL_SRCS})
target_link_libraries( HGEngineImpl HGCore )
