#include "EngineImpl.h"
//
// Created by cyf-m on 2020/11/27.
//

#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "../Core/Log.h"
#include "../Core/Error.h"
#include "../engine/HGInput.hpp"
#include "EngineImpl.h"
#include "Camera.h"
#include "Window.h"
#include "Init.h"
#include "Thread.h"
#include "Scene.h"
#include "GameObject.h"
#include "Asset.h"

using namespace HGEngine::V1SDL;
using namespace HGEngine;
using namespace HG;

static int _UpdateThreadFn( void* data );
static int _RenderThreadFn( void* data );

EngineImpl* EngineImpl::pEngine = nullptr;

int EngineImpl::Run() {
	pUpdateThread = new Thread( _UpdateThreadFn, "UPDATE", &( this->tLoopUpdate ), false );
	pRenderThread = new Thread( _RenderThreadFn, "RENDER", &( this->tLoopRender ), false );
	tLoopMain.Run();
	return 0; // for return tEngine.Run(); in main()
}

void InitSDLImage() {
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int res = IMG_Init( flags );
	if( ( res & flags ) != flags ) {
		HG_LOG_FAILED( "failed to init required jpg and png support! Details next log." );
		HG_LOG_FAILED( IMG_GetError() );
		exit( -1 );
	} else {
		HG_LOG_SUCCESS( "IMG_Init" );
	}
}

void InitSDLTtf() {
	if( TTF_Init() < 0 ) {
		HG_LOG_FAILED( "Error initializing SDL_ttf: " );
		HG_LOG_FAILED( TTF_GetError() );
		exit( -1 );
	} else {
		HG_LOG_SUCCESS( "TTF_Init" );
	}
}

EngineImpl::EngineImpl( int argc, char** argv )
	: pCurrentScene( nullptr ), pWindow( nullptr ), pUpdateThread( nullptr ), pRenderThread( nullptr ), pRenderer( nullptr ), pAsset( nullptr ), pInput( new HGInput ) {
	SetEngine( this );

	Init init;
	init.App()->SDL();
	InitSDLImage();
	InitSDLTtf();

	Log->Info( SDL_LOG_CATEGORY_SYSTEM, "Starting HoneyGame Engine ..." );

	pWindow = new Window( "HG Engine", 0, 0, 800, 640, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN );
	pWindow->SetCenterScreen();

	pRenderer = new Renderer2D( pWindow->Handle() );

	tLoopUpdate.unPaddingInterval = 100;
	tLoopUpdate.unRunInterval = 50;

	tLoopRender.unPaddingInterval = 100;
	tLoopRender.unRunInterval = 1;  // v-sync here, frame locked to screen fresh-rate.

	tLoopMain.unPaddingInterval = 100;
	tLoopMain.unRunInterval = 2; // for high quality input

	pCurrentScene = static_cast<Scene*>( Scene::umTheseOnes.begin()->second );

	pAsset = new Asset();
}

EngineImpl::~EngineImpl() {
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();

	HG_SAFE_DEL( pUpdateThread );
	HG_SAFE_DEL( pRenderThread );
	HG_SAFE_DEL( pRenderer );
	HG_SAFE_DEL( pWindow );
	HG_SAFE_DEL( pInput );
	EngineImpl::pEngine = nullptr;
}

void EngineImpl::Exit() {
	tLoopRender.eStatus = Loop::STOP;
	tLoopUpdate.eStatus = Loop::STOP;
	while( !tLoopRender.IsExit() || !tLoopUpdate.IsExit() ) {
		SDL_Delay( 100 );
	}
	tLoopMain.eStatus = Loop::STOP;
}

void EngineImpl::NavigateScene( const char* strSceneName ) {
	pCurrentScene = static_cast<Scene*>( Scene::Find( strSceneName ) );
	if( pCurrentScene != nullptr ) {
		HG_EVENT_CALLRAW_NO_DATA( pCurrentScene->OnAttach, pCurrentScene );
	} else {
		HG_LOG_FAILED( std::string( "no such scene named:" ).append( strSceneName ).c_str() );
	}
}

Scene* HGEngine::V1SDL::EngineImpl::FindScene( const char* strSceneName ) {
	return Scene::Find( strSceneName );
}


/// =========
/// main loop
/// =========
SDL_Event HGMainLoop::tEvent = SDL_Event();

// https://wiki.libsdl.org/SDL_Keycode
// https://wiki.libsdl.org/SDL_Event
void HGMainLoop::_RunTask() {
	( SDL_PollEvent( &HGMainLoop::tEvent ) != 0 ) ;
	{
		HG_ENGINE_INPUT()->Proc( &tEvent );
		EngineImpl::GetEngine()->GetCurrentScene()->Update( ( void* ) &HGMainLoop::tEvent );
	}
}

void HGMainLoop::_PaddingTask() { }

void HGMainLoop::_StopTask() { }

/// =========
/// update loop
/// =========

static int _UpdateThreadFn( void* data ) {
	auto pUpdateLoop = static_cast< HGUpdateLoop* >( data );
	pUpdateLoop->Run();
	return HG_ERR_OK;
}

void HGUpdateLoop::_RunTask() { 
	// EngineImpl::GetEngine()->GetCurrentScene()->Update();
}

void HGUpdateLoop::_PaddingTask() { }

void HGUpdateLoop::_StopTask() { }

/// =========
/// render loop
/// =========

static int _RenderThreadFn( void* data ) {
	auto pRenderLoop = static_cast< HGRenderLoop* >( data );
	pRenderLoop->Run();
	return HG_ERR_OK;
}

void HGRenderLoop::_RunTask() {
	auto pRd2D = EngineImpl::GetEngine()->GetRenderer2D();
	pRd2D->Clear( 100, 100, 100, 255 );
	EngineImpl::GetEngine()->GetCurrentScene()->Render( pRd2D );
	pRd2D->Present();
}

void HGRenderLoop::_PaddingTask() { }

void HGRenderLoop::_StopTask() { }
