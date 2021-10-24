//
// Created by cyf-m on 2020/12/9.
//

#include <SDL_image.h>
#include <SDL.h>
#include <Log.h>
#include <Math.hpp>
#include "GameObject2D.h"
#include "Renderer2D.h"
#include "EngineImpl.h"
#include "Scene.h"
#include "Transform.hpp"
#include "Texture.h"

using namespace HGEngine::V1SDL;
using namespace HG::Math;

SDL_Texture* HGEngine::V1SDL::GameObject2D::GetTexture() const {
	return m_pTexture->GetHandle();
}

HGSize<int> GameObject2D::GetTextureSize() {
	HGSize<int> size;
	SDL_QueryTexture( m_pTexture->GetHandle(), NULL, NULL, &size.W, &size.H );
	return size;
}

HGEngine::V1SDL::GameObject2D::GameObject2D( const char* strObjectName, Texture* pTexture )
	: GameObject( strObjectName ), m_pTexture( pTexture ) { }

GameObject2D::~GameObject2D() { }

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

void HGEngine::V1SDL::GameObjectText::Update( void* pEvent ) {
	HG_EVENT_CALL( OnFixedUpdate, pEvent, this );
}

void HGEngine::V1SDL::GameObjectText::Render( void* pRenderer ) {
	SDL_FreeSurface( m_pText );
	if( m_pTexture != nullptr ) {
		SDL_DestroyTexture( m_pTexture );
	}
	m_pText = TTF_RenderText_Solid( ( TTF_Font* ) m_pFont->GetHandle(), Text.c_str(), tColor );
	if( !m_pText ) {
		HG_LOG_FAILED( "Failed to render text: " );
		HG_LOG_FAILED( TTF_GetError() );
	}
	if( m_pText != nullptr ) {
		auto pRd2D = static_cast< Renderer2D* >( pRenderer );
		m_pTexture = SDL_CreateTextureFromSurface( pRd2D->pH, m_pText );

		HG_EVENT_CALL( OnRender, pRd2D, this );
		renderCameraView( pRd2D );
	}
}

HGEngine::V1SDL::GameObjectText::GameObjectText( const char* strObjectName, Font* pFont, const char* text )
	: GameObject2D( strObjectName, nullptr ), tColor( { 0, 0, 0 } ), Text( text ), m_pFont( pFont ), m_pText( nullptr ) { }

HGEngine::V1SDL::GameObjectText::~GameObjectText() {
	if( m_pTexture != nullptr ) {
		SDL_DestroyTexture( m_pTexture );
	}
	if( m_pText != nullptr ) {
		SDL_FreeSurface( m_pText );
	}
}


