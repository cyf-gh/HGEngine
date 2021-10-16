//
// Created by cyf-m on 2020/11/25.
//

#include <Error.h>
#include <Math.hpp>
#include <Log.h>
#include "Window.h"

using namespace HGEngine;
using namespace HGEngine::V1SDL;
using namespace HG::Math;
using namespace HG;

Window::Window(
        const char *title,
        int X, int Y, int w,
        int h, Uint32 flags ) {
    this->pWin = SDL_CreateWindow( title, X, Y, w, h, flags );
    HG_LOG_CHECK_SDL_HANDLE_IS_NULL(this->pWin, SDL_LOG_CATEGORY_SYSTEM, "SDL_CreateWindow");
}

Window::~Window() {
    SDL_DestroyWindow( this->pWin );
}

HGResult Window::GetSize(HGSize<un32>* pSize ) {
    int w = 0, h = 0;
    SDL_GetWindowSize( this->pWin, &w, &h );
    if ( w == 0 && h == 0 ) {
        return HG_ERR_NULL_OUTPUT;
    }
    pSize->H = h;
    pSize->W = w;
    return HG_ERR_OK;
}

HGResult Window::SetCenterScreen() {
    HGPos tPos;
    HGSize<un32> tSize;
    HG_RESULT_RES

    HG_CHECK_ERR(GetSize(&tSize ) )
    HG_CHECK_ERR(tScreen.GetCenterPos(tSize.W, tSize.H, &tPos ) )
    SDL_SetWindowPosition( pWin, tPos.X, tPos.Y );
    return HG_ERR_OK;
}


