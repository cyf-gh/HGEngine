//
// Created by cyf-m on 2020/12/9.
//

#include <SDL_image.h>
#include <SDL.h>
#include "GameObject2D.h"
#include "Renderer2D.h"
#include "EngineImpl.h"
#include "../Core/Log.h"

using namespace __HGImpl::V1SDL;

GameObject2D::GameObject2D( const char* strObjectName, const char* strFileName )
	: GameObject( strObjectName ), m_pTexture( nullptr ), strFileName( strFileName ) {
	AddComponent( new Figure("SrcFigure") );
	AddComponent( new Figure("DesFigure") );
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

void GameObject2D::Update( void* pEvent ) {
	HG_EVENT_CALL( OnUpdate, pEvent );
}

void GameObject2D::Render( void* pRenderer ) {
	auto pSrcFigure = GetComponent<Figure>( "SrcFigure" );
	auto pDestFigure = GetComponent<Figure>( "DesFigure" );

	auto pRd2D = static_cast< Renderer2D* >( pRenderer );
	HG_EVENT_CALL( OnRender, pRd2D );
	auto tSrcRect = pSrcFigure->ToSDLRect();
	auto tDestRect = pDestFigure->ToSDLRect();
	pRd2D->Copy( this, pSrcFigure->IsZero() ? nullptr : &tSrcRect, pDestFigure->IsZero() ? nullptr : &tDestRect );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

void __HGImpl::V1SDL::GameObjectText::Update( void* pEvent ) {
	HG_EVENT_CALL( OnUpdate, pEvent );
}

void __HGImpl::V1SDL::GameObjectText::Render( void* pRenderer ) { 
	m_pText = TTF_RenderText_Solid( m_pFont->pHandle, Text.c_str(), tColor );
	if( !m_pText ) {
		HG_LOG_FAILED( "Failed to render text: " );
		HG_LOG_FAILED( TTF_GetError() );
	}
	if( m_pText != nullptr ) {
		auto pRd2D = static_cast< Renderer2D* >( pRenderer );
		auto pSrcFigure = GetComponent<Figure>( "SrcFigure" );
		auto pDestFigure = GetComponent<Figure>( "DesFigure" );

		m_pTexture = SDL_CreateTextureFromSurface( pRd2D->pHandle, m_pText );
		auto tSrcRect = pSrcFigure->ToSDLRect();
		auto tDestRect = pDestFigure->ToSDLRect();
		pRd2D->Copy( this, pSrcFigure->IsZero() ? nullptr : &tSrcRect, pDestFigure->IsZero() ? nullptr : &tDestRect );
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


