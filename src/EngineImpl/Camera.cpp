#include <Log.h>
#include "Camera.h"
#include "EngineImpl.h"
#include "../Engine/HGBehaviour.h"
#include "Transform.hpp"

using namespace HGEngine::V1SDL;
using namespace HG;

HGEngine::V1SDL::Camera::Camera( const char* strName, Scene* pScene ) : GameObject( strName ), RenderInViewOnly( true ), pTarget( nullptr ) { }

void HGEngine::V1SDL::Camera::Update( void* pEvent ) {
	HG_EVENT_CALL( OnFixedUpdate, pEvent, this );
}

void HGEngine::V1SDL::Camera::Render( void* pRenderer ) {
	HG_EVENT_CALL( OnRender, pRenderer, this );
}

void Camera::SetCameraSizeToRendererSize() {
	auto pRenderer = EngineImpl::GetEngine()->GetRenderer2D();
	auto pT = GetComponent<Transform>();
	int w, h;
	if( 0 != SDL_GetRendererOutputSize( pRenderer->pHandle, &w, &h ) ) {
		HG_LOG_FAILED( "Failed SetCameraSizeToRendererSize -> SDL_GetRendererOutputSize" );
		HG_LOG_FAILED( SDL_GetError() );
		return;
	}
	pT->tRect.W = w; pT->tRect.H = h;
}
