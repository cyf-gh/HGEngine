//
// Created by cyf-m on 2020/12/10.
//

#include <string>
#include <SDL_image.h>
#include <Math.hpp>
#include <Log.h>
#include "Spirte.hpp"
#include "Renderer2D.h"
#include "Texture.h"
#include "Label.hpp"

using namespace HGEngine::V1SDL;
using namespace HG::Math;

SDL_Texture* Renderer2D::CreateTextureFromFile( const char* pStrFileName ) {
	auto pImg = IMG_Load( pStrFileName );
	if( !pImg ) {
		HG_LOG_FAILED( std::format( "Texture|{}| ?Failed Creation \n[{}]",  IMG_GetError()).c_str() );
		return nullptr;
	} else {
		HG_LOG_SUCCESS( std::format( "Texture|{}| !Created", pStrFileName ).c_str() );
	}

	auto pTex = SDL_CreateTextureFromSurface( pHandle, pImg );
	if( pTex == nullptr ) {
		HG_LOG_SDL_ERROR( SDL_LOG_CATEGORY_RENDER, "SDL_CreateTextureFromSurface in Renderer2D::CreateTextureFromFile" );
		return nullptr;
	}

	SDL_FreeSurface( pImg );
	pImg = nullptr;

	return pTex;
}

void Renderer2D::Copy( const GameObject* pGameObject, const SDL_Rect* pSrcRect, const SDL_Rect* pDstRect ) {
	if( !pGameObject ) {
		return;
	}
	auto pSp = pGameObject->GetComponent<Spirte>();
	if( pSp != nullptr ) {
		SDL_RenderCopy( pHandle, pSp->GetTexture()->GetHandle(), pSrcRect,  pDstRect );
	}
	auto pLb = pGameObject->GetComponent<Label>();
	if( pLb != nullptr ) {
		if ( pLb->RenderText2Texture( this ) ) {
			SDL_RenderCopy( pHandle, pLb->GetTexture(), pSrcRect, pDstRect );
		}
	}
}

void HGEngine::V1SDL::Renderer2D::CopyEx( const GameObject* pGameObject, const SDL_Rect* pSrcRect, const SDL_Rect* pDstRect, const double f64Angle, const SDL_Point* pCenter, const SDL_RendererFlip& tFlip ) { 
	if( !pGameObject ) {
		return;
	}
	auto pSp = pGameObject->GetComponent<Spirte>();
	if( pSp != nullptr ) {
		SDL_RenderCopyEx( pHandle, pSp->GetTexture()->GetHandle(), pSrcRect,  pDstRect, f64Angle, pCenter, tFlip );
	}
	auto pLb = pGameObject->GetComponent<Label>();
	if( pLb != nullptr ) {
		if( pLb->RenderText2Texture( this ) ) {
			SDL_RenderCopy( pHandle, pLb->GetTexture(), pSrcRect, pDstRect );
		}
	}
}
