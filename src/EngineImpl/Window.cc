//
// Created by cyf-m on 2020/11/25.
//

#include <Error.h>
#include <Math.hpp>
#include <Log.h>
#include "Window.h"
#include "EngineImpl.h"

using namespace HGEngine;
using namespace HGEngine::V1SDL;
using namespace HG::Math;
using namespace HG;


#pragma region Watchers

static int resizingEventWatcher( void* data, SDL_Event* event ) {
    if( event->type == SDL_WINDOWEVENT &&
        event->window.event == SDL_WINDOWEVENT_RESIZED ) {
        SDL_Window* win = SDL_GetWindowFromID( event->window.windowID );
        auto* pwin = HG_ENGINE()->GetWindow();

        if( win == ( SDL_Window* ) data ) {
            switch( pwin->tResize ) {
            case HGEngine::V1SDL::Window::Scaling:

            break;
            case HGEngine::V1SDL::Window::Expanding:
            //auto tsize = pwin->GetSize();
            //HG_ENGINE()->GetRenderer2D()->SetViewport( tsize.W, tsize.H );
            //HG_ENGINE()->GetRenderer2D()->SetScale( tsize.W, tsize.H );
            break;
            }
        }
    }
    return 0;
}
#pragma endregion

Window::Window(
        const char *title,
        int X, int Y, int w,
        int h, Uint32 flags ) : tResize( eResize::Expanding ) {
    this->pWin = SDL_CreateWindow( title, X, Y, w, h, flags );
    SDL_AddEventWatch( resizingEventWatcher, pWin );
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

HG::Math::HGSize<un32> HGEngine::V1SDL::Window::GetSize() {
    int w = 0, h = 0;
    SDL_GetWindowSize( this->pWin, &w, &h );
    HG::Math::HGSize<un32> size( w, h );
    return size;
}



HGResult Window::SetCenterScreen() {
    HGPos tPos;
    HGSize<un32> tSize;
    HG_RESULT_RES;

    HG_CHECK_ERR(GetSize(&tSize ) )
    HG_CHECK_ERR(tScreen.GetCenterPos(tSize.W, tSize.H, &tPos ) )
    SDL_SetWindowPosition( pWin, tPos.X, tPos.Y );
    return HG_ERR_OK;
}
