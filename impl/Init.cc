//
// Created by cyf-m on 2020/11/25.
//

#include "../include/Init.h"
#include <SDL.h>

void HG::Init::SDL() {
    if ( SDL_Init(SDL_INIT_EVERYTHING ) == -1){
        SDL_LogError( SDL_LOG_CATEGORY_SYSTEM, "SDL_Init failed: %s", SDL_GetError());
    } else {
        SDL_LogInfo( SDL_LOG_CATEGORY_SYSTEM, "SDL_Init success" );
    }
}