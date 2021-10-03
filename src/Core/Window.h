//
// Created by cyf on 2020/11/25.
//

#ifndef HONEYGAME_WINDOW_H
#define HONEYGAME_WINDOW_H
#include <SDL.h>
#include "Error.h"
#include "Screen.h"
#include "Memory.h"

namespace __HGImpl {
namespace V1SDL {
class Window : HG::Memory::NonCopyable {
    private:
        SDL_Window *pWin;
        Screen tScreen;
    public:
        SDL_Window* Handle() { return pWin; };
        HG::HGResult SetCenterScreen();
        HG::HGResult GetSize( IN_OUT HG::Math::HGSize* pSize );

        Window(const char *title,
               int X, int Y, int w,
               int h, Uint32 flags = SDL_WINDOW_SHOWN);
        virtual ~Window();
};
}
}
#endif // HONEYGAME_WINDOW_H
