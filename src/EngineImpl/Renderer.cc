//
// Created by cyf-m on 2020/11/26.
//

#include <SDL.h>
#include "Log.hpp"
#include "Renderer.h"

using namespace HGEngine::V1SDL;
using namespace HG;

Renderer::Renderer( SDL_Window * window, int index, Uint32 flags ) {
    this->pHandle = SDL_CreateRenderer( window, index, flags );
    HG_LOG_CHECK_SDL_HANDLE_IS_NULL(this->pHandle, SDL_LOG_CATEGORY_SYSTEM, "Renderer::Renderer");
}

Renderer::~Renderer() {
    SDL_DestroyRenderer( pHandle );
}

void Renderer::Present() {
    SDL_RenderPresent( pHandle );
}

void Renderer::Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    UINT8 rr, gg, bb, aa;
    SDL_GetRenderDrawColor( pHandle, &rr, &gg, &bb, &aa );
    SDL_SetRenderDrawColor( pHandle, r, g, b, a );
    SDL_RenderClear( pHandle );
    SDL_SetRenderDrawColor( pHandle, rr, gg, bb, aa );
}

int HGEngine::V1SDL::Renderer::SetScale( f32 scaleX, f32 scaleY ) { 
    return SDL_RenderSetScale( pHandle, scaleX, scaleY );
}

int HGEngine::V1SDL::Renderer::SetViewport( f32 w, f32 h ) {
    SDL_Rect r;
    r.x = 0; r.y = 0; r.w = static_cast<int>(w); r.h = static_cast< int >( h);
    return SDL_RenderSetViewport( pHandle, &r );
}
