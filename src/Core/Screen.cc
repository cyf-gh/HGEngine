//
// Created by cyf-m on 2020/11/25.
//
#include <SDL.h>
#include "Screen.h"
#include "Log.h"

using namespace HGCore;
using namespace HGCore::Math;

HGResult Screen::GetCenterPos(int w, int h, HGPos *pCenterPos, int displayIndex) {
    SDL_DisplayMode dm;

    SDL_LogInfo( SDL_LOG_CATEGORY_SYSTEM, "Get center position of window index: %d", displayIndex );
    if ( SDL_GetDesktopDisplayMode(displayIndex, &dm) != 0 ) {
        Log->FailedSDL(SDL_LOG_CATEGORY_SYSTEM, "SDL_GetDesktopDisplayMode");
        return HG_ERR_NULL_OUTPUT;
    }

    pCenterPos->X = dm.w / 2 - w / 2;
    pCenterPos->Y = dm.h / 2 - h / 2;
    return HG_ERR_OK;
}
