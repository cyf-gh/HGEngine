//
// Created by cyf-m on 2020/11/25.
//

#include "../include/Window.h"
#include "../include/Error.h"
#include "../include/Math.h"

using namespace HG;
using namespace HG::Math;

Window::Window() {
    this->pWin = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (this->pWin == nullptr){
        SDL_LogError( SDL_LOG_CATEGORY_SYSTEM, "SDL_CreateWindow failed: %s", SDL_GetError());
    } else {

    }
}

HGResult Window::SetCenterScreen() {
    HGPos tPos;
    HGResult res;

    res = tScreen.GetCenterPos( 640, 480, &tPos );
    SDL_SetWindowPosition( pWin, tPos.X, tPos.Y );
    return res;
}

