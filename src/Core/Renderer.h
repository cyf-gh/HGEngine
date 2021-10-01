//
// Created by cyf-m on 2020/11/26.
//

#ifndef HONEYGAME_RENDERER_H
#define HONEYGAME_RENDERER_H

#include <SDL_render.h>
#include "Memory.h"

namespace __HGImpl {

class Renderer {
    HG_MEM_NO_BINARY_COPY( Renderer );
private:
public:
    SDL_Renderer *pHandle = nullptr;
    void Present();
    void Clear( Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);
    Renderer(
            SDL_Window* window,
            int index = -1,
            Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    virtual ~Renderer();
};

}

#endif //HONEYGAME_RENDERER_H
