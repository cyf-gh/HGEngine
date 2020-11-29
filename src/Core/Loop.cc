//
// Created by cyf-m on 2020/11/27.
//

#include <exception>
#include "Loop.h"
#include "Log.h"
using namespace HGCore;

void Loop::Run() {
    while( true ) {
        switch ( eStatus ) {
            case RUN:
                this->_RunTask();
                // SDL_Event event; while ( SDL_PollEvent( &event ) ) {switch ( event.type ) {}}
                SDL_Delay( unRunInterval );
                break;
            case STOP:
                this->_StopTask();
                goto THREAD_EXIT; // break;
            case PADDING:
                this->_PaddingTask();
                SDL_Delay( unPaddingInterval );
                break;
            default:
                Log->Fault( SDL_LOG_CATEGORY_SYSTEM, "Unknown loop status. loop break." );
                return;
        }
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
