//
// Created by cyf-m on 2020/11/25.
//

#include <SDL.h>
#include <iostream>
#include <Log.h>
#include "Init.h"

using namespace HGEngine::V1SDL;
using namespace HG;

Init* Init::SDL() {
    if ( SDL_Init(SDL_INIT_EVERYTHING ) == -1 ){
        Log->FailedSDL(SDL_LOG_CATEGORY_SYSTEM, "SDL_Init");
    } else {
        Log->Success( SDL_LOG_CATEGORY_SYSTEM, "SDL_Init" );
    }
    return this;
}

Init* Init::App() {
    Log->LogEnter2File();
    return this;
}
