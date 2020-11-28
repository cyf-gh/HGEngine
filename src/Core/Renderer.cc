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
