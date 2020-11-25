//
// Created by cyf-m on 2020/11/25.
//
#include <SDL.h>
#include "../include/Screen.h"

HG::HGResult HG::Screen::GetCenterPos(int w, int h, HG::Math::HGPos *pOutCenterPos, int displayIndex) {
    SDL_DisplayMode dm;

    SDL_LogInfo( SDL_LOG_CATEGORY_SYSTEM, "Get center position of window index: %d", displayIndex);
    if ( SDL_GetDesktopDisplayMode(displayIndex, &dm) != 0 ) {
        SDL_LogError( SDL_LOG_CATEGORY_SYSTEM, "SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return HG_ERR_NULL_OUTPUT;
    }

    pOutCenterPos->X = dm.w / 2 - w / 2;
    pOutCenterPos->Y = dm.h / 2 - h / 2;
    return HG_ERR_OK;
}
