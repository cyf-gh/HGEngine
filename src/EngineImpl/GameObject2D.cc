//
// Created by cyf-m on 2020/12/9.
//

#include <SDL_image.h>
#include <SDL.h>
#include "GameObject2D.h"
#include "Renderer2D.h"
#include "EngineImpl.h"
#include "../Core/Log.h"
#include "Transform.hpp"
#include "Scene.h"

using namespace __HGImpl::V1SDL;

GameObject2D::GameObject2D( const char* strObjectName, const char* strFileName )
	: GameObject( strObjectName ), m_pTexture( nullptr ), strFileName( strFileName ) {
	AddComponent( new Transform("Transform") );
	if( strlen( strFileName ) != 0 ) {
		m_pTexture = EngineImpl::GetEngine()->GetRenderer2D()->CreateTextureFromFile( strFileName );
	}
}

GameObject2D::~GameObject2D() {
	if( m_pTexture != nullptr ) {
		auto* pEngine = EngineImpl::GetEngine();
		if( pEngine != nullptr ) {
			pEngine->GetRenderer2D()->m_umTextures[strFileName] = nullptr;
		}
		SDL_DestroyTexture( m_pTexture );
	}
}

void GameObject2D::renderCameraView( Renderer2D* pRenderer ) {
	auto pTransform = GetComponent<Transform>();
	auto tSrcRect = pTransform->ToSDLRectLocal();
	auto tDestRect = pTransform->ToSDLRectGlobal();
	auto tCenterPt = pTransform->ToSDLPoint();
	if( m_pScene != nullptr ) {
		auto pCam = m_pScene->GetMainCamera();
		if( pCam != nullptr ) {
			auto pCamTransform = pCam->GetComponent<Transform>();
			if( pCam->RenderInViewOnly && !this->IsInCameraView() ) {
				return;
			}
			tDestRect.x -= pCamTransform->tPosition.X;
			tDestRect.y -= pCamTransform->tPosition.Y;
		}
	}
	pRenderer->CopyEx( this, pTransform->IsZeroLocal() ? nullptr : &tSrcRect, pTransform->IsZeroGlobal() ? nullptr : &tDestRect, pTransform->f64Angle, &tCenterPt, SDL_FLIP_NONE );
}

void GameObject2D::Update( void* pEvent ) {
	HG_EVENT_CALL( OnFixedUpdate, pEvent, this );
}

void GameObject2D::Render( void* pRenderer ) {
	auto pRd2D = static_cast< Renderer2D* >( pRenderer );
	HG_EVENT_CALL( OnRender, pRd2D, this );
	renderCameraView( pRd2D );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

void __HGImpl::V1SDL::GameObjectText::Update( void* pEvent ) {
	HG_EVENT_CALL( OnFixedUpdate, pEvent, this );
}

void __HGImpl::V1SDL::GameObjectText::Render( void* pRenderer ) { 
	m_pText = TTF_RenderText_Solid( m_pFont->pHandle, Text.c_str(), tColor );
	if( !m_pText ) {
		HG_LOG_FAILED( "Failed to render text: " );
		HG_LOG_FAILED( TTF_GetError() );
	}
	if( m_pText != nullptr ) {
		auto pRd2D = static_cast< Renderer2D* >( pRenderer );
		m_pTexture = SDL_CreateTextureFromSurface( pRd2D->pHandle, m_pText );

		HG_EVENT_CALL( OnRender, pRd2D, this );
		renderCameraView( pRd2D );
	}
}

__HGImpl::V1SDL::GameObjectText::GameObjectText( const char* strObjectName, Font* pFont, const char* text )
	: GameObject2D( strObjectName, "" ), tColor( { 0, 0, 0 } ), Text( text ), m_pFont( pFont ), m_pText( nullptr ) { }

__HGImpl::V1SDL::GameObjectText::~GameObjectText() { 
	if( m_pTexture != nullptr ) {
		SDL_DestroyTexture( m_pTexture );
	}
	if( m_pText != nullptr ) {
		SDL_FreeSurface( m_pText );
	}
}


