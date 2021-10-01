//
// Created by cyf-m on 2020/11/25.
//

#include "Window.h"
#include "Error.h"
#include "Math.h"
#include "Log.h"

using namespace __HGImpl;
using namespace __HGImpl::Math;

Window::Window(
        const char *title,
        int x, int y, int w,
        int h, Uint32 flags ) {
    this->pWin = SDL_CreateWindow( title, x, y, w, h, flags );
    HG_LOG_CHECK_SDL_HANDLE_IS_NULL(this->pWin, SDL_LOG_CATEGORY_SYSTEM, "SDL_CreateWindow");
}

Window::~Window() {
    SDL_DestroyWindow( this->pWin );
}

HGResult Window::GetSize(HGSize* pSize ) {
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
    HGSize tSize;
    HG_RESULT_RES

    HG_CHECK_ERR(GetSize(&tSize ) )
    HG_CHECK_ERR(tScreen.GetCenterPos(tSize.W, tSize.H, &tPos ) )
    SDL_SetWindowPosition( pWin, tPos.X, tPos.Y );
    return HG_ERR_OK;
}


