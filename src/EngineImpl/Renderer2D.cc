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
#include "GameObject.h"
#include "Geometry.hpp"

using namespace HGEngine::V1SDL;
using namespace HG::Math;

SDL_Texture* Renderer2D::CreateTextureFromFile( const char* pStrFileName ) {
	auto pImg = IMG_Load( pStrFileName );
	if( !pImg ) {
		HG_LOG_FAILED( std::format( "Texture|{}| ?Failed Creation \n[{}]", IMG_GetError() ).c_str() );
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
	CopyEx( pGameObject, pSrcRect, pDstRect, 0, nullptr, SDL_FLIP_NONE );
}

void HGEngine::V1SDL::Renderer2D::CopyEx( const GameObject* pGameObject, const SDL_Rect* pSrcRect, const SDL_Rect* pDstRect, const double f64Angle, const SDL_Point* pCenter, const SDL_RendererFlip& tFlip ) {
	if( !pGameObject ) {
		return;
	}
	auto vecRC = pGameObject->GetRenderableComponentsSorted();
	for( auto& rc : vecRC ) {
		if( rc != nullptr ) {
			auto* pRt = rc->GetRenderTarget( this );
			auto* pLR = rc->GetLocalRectOffset();
			if( pRt != nullptr ) {
				if( pLR == nullptr ) {
					SDL_RenderCopyEx( pHandle, pRt->GetHandle(), pSrcRect, pDstRect, f64Angle, pCenter, tFlip );
				} else {
					if( pLR->IsZero() ) {
						SDL_RenderCopyEx( pHandle, pRt->GetHandle(), pSrcRect, pDstRect, f64Angle, pCenter, tFlip );
					} else {
						SDL_Rect r;
						r.x = pLR->X + pDstRect->x;
						r.y = pLR->Y + pDstRect->y;
						r.w = pLR->W;
						r.h = pLR->H;
						SDL_RenderCopyEx( pHandle, pRt->GetHandle(), pSrcRect, &r, f64Angle, pCenter, tFlip );
					}
				}
			}
		}
	}
}

void HGEngine::V1SDL::Renderer2D::SetDrawColor( Uint8 r, Uint8 g, Uint8 b, Uint8 a ) {
	SDL_SetRenderDrawColor( pHandle, r, g, b, a );
}

void HGEngine::V1SDL::Renderer2D::SetDrawColor( const DrawableGeo* pdg ) {
	SDL_SetRenderDrawColor( pHandle, pdg->r, pdg->g, pdg->b, pdg->a );
}

void HGEngine::V1SDL::Renderer2D::SetDrawColor( const HG::Math::HGColor& tc ) {
	SetDrawColor( tc.R, tc.G, tc.B, tc.A );
}

void HGEngine::V1SDL::Renderer2D::SetDrawBlendMode( const SDL_BlendMode eBm ) {
	SDL_SetRenderDrawBlendMode( pHandle, eBm );
}

int HGEngine::V1SDL::Renderer2D::DrawLine( int x, int y, int xx, int yy ) {
	return SDL_RenderDrawLine( pHandle, x, y, xx, yy );
}
