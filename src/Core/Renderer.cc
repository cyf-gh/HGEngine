//
// Created by cyf-m on 2020/11/26.
//

#include "Renderer.h"
#include "Log.h"
using namespace HGCore;

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
