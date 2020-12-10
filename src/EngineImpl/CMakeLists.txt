set( SDL2_IMAGE_PATH "L:/SDL2_image-2.0.5/x86_64-w64-mingw32")

find_package( SDL2 REQUIRED )
find_package( SDL2_image REQUIRED )

if ( NOT SDL2_IMAGE_FOUND )
    message(FATAL_ERROR "SDL_IMAGE not found!")
endif( NOT SDL2_IMAGE_FOUND )

include_directories( ${SDL2_INCLUDE_DIR} )
include_directories( ${SDL2_IMAGE_INCLUDE_DIRS} )

aux_source_directory(. DIR_ENGINE_IMPL_SRCS)
add_library ( EngineImpl ${DIR_ENGINE_IMPL_SRCS} )
target_link_libraries( EngineImpl HGCore )
target_link_libraries( EngineImpl ${SDL2_IMAGE_LIBRARIES} )

