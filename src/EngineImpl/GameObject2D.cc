//
// Created by cyf-m on 2020/12/9.
//

#include <SDL_image.h>
#include <SDL.h>
#include "GameObject2D.h"
#include "Renderer2D.h"
#include "EngineImpl.h"
#include "../Core/Log.h"

using namespace __HGImpl::V1;

GameObject2D::GameObject2D( const char* strObjectName, const char* strFileName )
	: GameObject( strObjectName ), m_pTexture( nullptr ), strFileName( strFileName ) {
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

}

void GameObject2D::Render( HGCore::Renderer* pRenderer ) {
	auto pRd2D = dynamic_cast< Renderer2D* >( pRenderer );
	auto tSrcRect = tSrcFigure.ToSDLRect();
	auto tDestRect = tDestFigure.ToSDLRect();
	pRd2D->Copy( this, tSrcFigure.IsZero() ? nullptr : &tSrcRect, tDestFigure.IsZero() ? nullptr : &tDestRect );
}

void GameObject2D::OnEnable() {

}

void GameObject2D::OnDisable() {

}

void GameObject2D::OnAttach() {

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

void __HGImpl::V1::GameObjectText::Update( void* pEvent ) {
}

void __HGImpl::V1::GameObjectText::Render( HGCore::Renderer* pRenderer ) { 
	m_pText = TTF_RenderText_Solid( m_pFont->pHandle, Text.c_str(), tColor );
	if( !m_pText ) {
		HG_LOG_FAILED( "Failed to render text: " );
		HG_LOG_FAILED( TTF_GetError() );
	}
	if( m_pText != nullptr ) {
		auto pRd2D = dynamic_cast< Renderer2D* >( pRenderer );
		m_pTexture = SDL_CreateTextureFromSurface( pRd2D->pHandle, m_pText );
		auto tSrcRect = tSrcFigure.ToSDLRect();
		auto tDestRect = tDestFigure.ToSDLRect();
		pRd2D->Copy( this, tSrcFigure.IsZero() ? nullptr : &tSrcRect, tDestFigure.IsZero() ? nullptr : &tDestRect );
	}
}

__HGImpl::V1::GameObjectText::GameObjectText( const char* strObjectName, Font* pFont, const char* text )
	: GameObject2D( strObjectName, "" ), tColor( { 0, 0, 0 } ), Text( text ), m_pFont( pFont ), m_pText( nullptr ) { }

__HGImpl::V1::GameObjectText::~GameObjectText() { 
	if( m_pTexture != nullptr ) {
		SDL_DestroyTexture( m_pTexture );
	}
	if( m_pText != nullptr ) {
		SDL_FreeSurface( m_pText );
	}
}


