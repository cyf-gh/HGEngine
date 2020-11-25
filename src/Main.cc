/*
 * @Date: 2020-11-24 11:50:12
 * @LastEditors: cyf
 * @LastEditTime: 2020-11-24 23:31:12
 * @FilePath: \HoneyGame\src\main.cc
 * @Description: What is mind? No matter. What is matter? Nevermind.
 */

#include <iostream>
#include <SDL.h>
#include "../include/Error.h"
#include "../include/Math.h"
using namespace HG;
using namespace Math;

int main( int argc, char **argv ) {

    SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);

    SDL_Delay(5000);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    std::cout<<"hello honey game!!!";
    std::cout<<"hello honey game!!!";
    return 0;
}


