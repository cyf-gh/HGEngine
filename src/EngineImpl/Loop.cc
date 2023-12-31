//
// Created by cyf-m on 2020/11/27.
//

#include <cmath>
#include "Log.hpp"
#include "EngineImpl.h"
#include "Loop.h"

using namespace HGEngine;
using namespace HG;
using namespace V1SDL;

void Loop::Render() {
    Uint64 un64Start = 0;
    Uint64 un64End = 0;
    unCurrentElapsedTimeMS = 0;
    // WARNNING: DO NOT ADD CODE HERE!!!    
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
                HGLog::Log->Fault( SDL_LOG_CATEGORY_SYSTEM, "Unknown loop status. loop break." );
                return;
        }
        un64End = SDL_GetPerformanceCounter();
        /// todo: asm
        f32CurrentElapsedTimeS = (un64End - un64Start) / (float)SDL_GetPerformanceFrequency();
        // f64CurrentElapsedTimeS = static_cast<double>( unCurrentElapsedTimeMS ) / static_cast<double>( SDL_GetPerformanceFrequency() ) / 1000;
    }
    THREAD_EXIT:
    m_IsExit = true;
    HGLog::Log->Info(SDL_LOG_CATEGORY_SYSTEM, std::format(  "{}::Run() -Exit", GetName() ).c_str() );
}

void Loop::_RunTask() {
    HGLog::Log->Warning(SDL_LOG_CATEGORY_SYSTEM, std::format( "In {}::_RunTask; nothing to do. Thread will be stopped.", GetName() ).c_str() );
    eStatus = STOP;
}

void Loop::_PaddingTask() {
    HGLog::Log->Warning( SDL_LOG_CATEGORY_SYSTEM, std::format( "In {}::_PaddingTask; nothing to do. Thread will be stopped.", GetName() ).c_str() );
    eStatus = STOP;
}

void Loop::_StopTask() {
    HGLog::Log->Warning( SDL_LOG_CATEGORY_SYSTEM, std::format( "In {}::_StopTask; nothing to do. Thread will be stopped.", GetName() ).c_str() );
}
