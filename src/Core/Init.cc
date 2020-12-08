//
// Created by cyf-m on 2020/11/25.
//

#include <SDL.h>
#include <iostream>
#include "Init.h"
#include "Log.h"

using namespace HGCore;

Init* HGCore::Init::SDL() {
    if ( SDL_Init(SDL_INIT_EVERYTHING ) == -1 ){
        Log->FailedSDL(SDL_LOG_CATEGORY_SYSTEM, "SDL_Init");
    } else {
        Log->Info( SDL_LOG_CATEGORY_SYSTEM, "SDL_Init" );
    }
    return this;
}

Init* Init::App() {
    Log->LogEnter2File();
    return this;
}
