//
// Created by cyf-m on 2020/11/27.
//

#include <cmath>
#include "Loop.h"
#include "Log.h"
using namespace HGCore;

void Loop::Run() {
    Uint64 un64Start = 0;
    Uint64 un64End = 0;
    f64CurrentElapsedTimeS = 0;
    un32 unCurrentElapsedTimeMS = 0;
    while( true ) {
        un64Start = SDL_GetPerformanceCounter();
        switch ( eStatus ) {
            case RUN:
                SDL_Delay( unCurrentElapsedTimeMS >= unRunInterval ? 0 : unRunInterval - unCurrentElapsedTimeMS );
                this->_RunTask();
                break;
            case STOP:
                this->_StopTask();
                goto THREAD_EXIT; // break;
            case PADDING:
                SDL_Delay( unCurrentElapsedTimeMS >= unPaddingInterval ? 0 : unPaddingInterval - unCurrentElapsedTimeMS );
                this->_PaddingTask();
                break;
            default:
                Log->Fault( SDL_LOG_CATEGORY_SYSTEM, "Unknown loop status. loop break." );
                return;
        }
        un64End = SDL_GetPerformanceCounter();
        /// todo: asm
        f64CurrentElapsedTimeS = static_cast<double>( un64End - un64Start ) / static_cast<double>(SDL_GetPerformanceFrequency());
        unCurrentElapsedTimeMS = ( un64End - un64Start ) / SDL_GetPerformanceFrequency();
    }
    THREAD_EXIT:
    Log->Info(SDL_LOG_CATEGORY_SYSTEM, "Thread exit");
}

void Loop::_RunTask() {
    Log->Warning(SDL_LOG_CATEGORY_SYSTEM, "In Loop::_RunTask; nothing to do. Thread will be stopped.");
    eStatus = STOP;
}

void Loop::_PaddingTask() {
    Log->Warning(SDL_LOG_CATEGORY_SYSTEM, "In Loop::_PaddingTask; nothing to do. Thread will be stopped.");
    eStatus = STOP;
}

void Loop::_StopTask() {
    Log->Warning(SDL_LOG_CATEGORY_SYSTEM, "In Loop::_StopTask; nothing to do. Thread will be stopped.");
}
