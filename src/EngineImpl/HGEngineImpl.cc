//
// Created by cyf-m on 2020/11/27.
//

#include <SDL_events.h>
#include "../Core/Log.h"
#include "HGEngineImpl.h"
#include "../Core/Error.h"
#include "../Core/Math.h"
#include "../Core/Log.h"
#include "../Core/Window.h"
#include "../Core/Init.h"
#include "../Core/Thread.h"

using namespace __HGImpl::V1;
using namespace HGCore;
static int _UpdateThreadFn( void* data );
static int _RenderThreadFn( void* data );

int HGEngineImpl::Run() {
    pUpdateThread = new Thread( _UpdateThreadFn, "UPDATE", &(this->tLoopUpdate), false );
    pRenderThread = new Thread( _RenderThreadFn, "RENDER", &(this->tLoopRender), false );
    tLoopMain.Run();
    return 0; // for return tEngine.Run(); in main()
}

HGEngineImpl::HGEngineImpl( int argc, char **argv )
: pUpdateThread( nullptr ), pRenderThread( nullptr ) {
    Init init;
    init.App();
    init.SDL();
    Log->Info(SDL_LOG_CATEGORY_SYSTEM, "Start HoneyGame Engine ...");

    pWindow = new Window( "HG Engine", 0, 0, 800, 640 );
    pWindow->SetCenterScreen();

    tLoopUpdate.unPaddingInterval = 100;
    tLoopUpdate.unRunInterval = 100;

    tLoopRender.unPaddingInterval = 100;
    tLoopRender.unRunInterval = 10;

    tLoopMain.unPaddingInterval = 100;
    tLoopMain.unRunInterval = 1; // for high quality input
}

HGEngineImpl::~HGEngineImpl() {
    HG_SAFE_DEL( pUpdateThread );
    HG_SAFE_DEL( pRenderThread );
}

/// =========
/// main loop
/// =========

void HGMainLoop::_RunTask() {
    SDL_Event event;
    while ( SDL_PollEvent( &event ) ) {
        switch ( event.type ) {

        }
    }
}

void HGMainLoop::_PaddingTask() {
}

void HGMainLoop::_StopTask() {
}

/// =========
/// update loop
/// =========

static int _UpdateThreadFn( void* data ) {
    auto pUpdateLoop = static_cast<HGUpdateLoop*>( data );
    pUpdateLoop->Run();
    return HG_ERR_OK;
}

void HGUpdateLoop::_RunTask() {
}

void HGUpdateLoop::_PaddingTask() {
}

void HGUpdateLoop::_StopTask() {
}

/// =========
/// render loop
/// =========

static int _RenderThreadFn( void* data ) {
    auto pUpdateLoop = static_cast<HGUpdateLoop*>( data );
    pUpdateLoop->Run();
    return HG_ERR_OK;
}

void HGRenderLoop::_RunTask() {
}

void HGRenderLoop::_PaddingTask() {
}

void HGRenderLoop::_StopTask() {
}
