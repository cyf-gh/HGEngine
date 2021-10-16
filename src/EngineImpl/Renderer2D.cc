//
// Created by cyf-m on 2020/12/10.
//

#include <string>
#include <SDL_image.h>
#include <Math.hpp>
#include <Log.h>
#include "Renderer2D.h"

using namespace HGEngine::V1SDL;
using namespace HG::Math;

SDL_Texture* Renderer2D::CreateTextureFromFile( const char* pStrFileName ) {
	auto pImg = IMG_Load( pStrFileName );
	if( !pImg ) {
		HG_LOG_FAILED( std::string( pStrFileName ).append( " -> create texture failed; details:" ).append( IMG_GetError() ).c_str() );
		return nullptr;
	} else {
		HG_LOG_SUCCESS( std::string( pStrFileName ).append( " -> create texture " ).c_str() );
	}

	auto pTex = SDL_CreateTextureFromSurface( pHandle, pImg );
	if( pTex == nullptr ) {
		HG_LOG_SDL_ERROR( SDL_LOG_CATEGORY_RENDER, "SDL_CreateTextureFromSurface in Renderer2D::CreateTextureFromFile" );
		return nullptr;
	} else {
		HG_LOG_SUCCESS( "SDL_CreateTextureFromSurface in Renderer2D::CreateTextureFromFile" );
	}

	SDL_FreeSurface( pImg );
	pImg = nullptr;

	m_umTextures[pStrFileName] = pTex;

	return pTex;
}

void Renderer2D::Copy( const GameObject2D* pGameObject, const SDL_Rect* pSrcRect, const SDL_Rect* pDstRect ) {
	if( !pGameObject ) {
		return;
	}
	SDL_RenderCopy( pHandle, pGameObject->GetTexture(), pSrcRect,  pDstRect );
}

void HGEngine::V1SDL::Renderer2D::CopyEx( const GameObject2D* pGameObject, const SDL_Rect* pSrcRect, const SDL_Rect* pDstRect, const double f64Angle, const SDL_Point* pCenter, const SDL_RendererFlip& tFlip ) { 
	if( !pGameObject ) {
		return;
	}
	SDL_RenderCopyEx( pHandle, pGameObject->GetTexture(), pSrcRect,  pDstRect, f64Angle, pCenter, tFlip );
}
