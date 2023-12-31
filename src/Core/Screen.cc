//
// Created by cyf-m on 2020/11/25.
//
#include <SDL.h>
#include "../EngineImpl/Log.hpp"
#include "Screen.h"

using namespace HGEngine;
using namespace HGEngine::V1SDL;
using namespace HG::Math;
using namespace HG;

HGResult Screen::GetCenterPos(int w, int h, HGPos *pCenterPos, int displayIndex) {
    SDL_DisplayMode dm;

    SDL_LogInfo( SDL_LOG_CATEGORY_SYSTEM, "Get center position of window index: %d", displayIndex );
    if ( SDL_GetDesktopDisplayMode(displayIndex, &dm) != 0 ) {
        HGLog::Log->FailedSDL(SDL_LOG_CATEGORY_SYSTEM, "SDL_GetDesktopDisplayMode");
        return HG_ERR_NULL_OUTPUT;
    }

    pCenterPos->X = dm.w / 2 - w / 2;
    pCenterPos->Y = dm.h / 2 - h / 2;
    return HG_ERR_OK;
}
