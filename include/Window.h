//
// Created by cyf on 2020/11/25.
//

#ifndef HONEYGAME_WINDOW_H
#define HONEYGAME_WINDOW_H
#include <SDL.h>
#include "Error.h"
#include "Screen.h"

namespace HG {

class Window {
    private:
        SDL_Window *pWin;
        Screen tScreen;
    public:
        Window();
        ~Window() {
            SDL_DestroyWindow( this->pWin );
        }

        HGResult SetCenterScreen();
};

}


#endif //HONEYGAME_WINDOW_H
