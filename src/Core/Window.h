//
// Created by cyf on 2020/11/25.
//

#ifndef HONEYGAME_WINDOW_H
#define HONEYGAME_WINDOW_H
#include <SDL.h>
#include "Error.h"
#include "Screen.h"

namespace HGCore {

class Window {
    private:
        SDL_Window *pWin;
        Screen tScreen;
    public:
        SDL_Window* Handle() { return pWin; };
        HGResult SetCenterScreen();
        HGResult GetSize( IN_OUT Math::HGRect* pSize );

        Window(const char *title,
               int x, int y, int w,
               int h, Uint32 flags = SDL_WINDOW_SHOWN);
        ~Window();
};

}
#endif // HONEYGAME_WINDOW_H
