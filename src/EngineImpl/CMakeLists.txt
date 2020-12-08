find_package( SDL2 REQUIRED )
include_directories( ${SDL2_INCLUDE_DIR} )

aux_source_directory(. DIR_ENGINE_IMPL_SRCS)
add_library ( EngineImpl ${DIR_ENGINE_IMPL_SRCS} )
target_link_libraries( EngineImpl HGCore )
