//
// Created by cyf-m on 2020/12/10.
//

#include <SDL_image.h>
#include <string>
#include "../Core/Math.h"
#include "../Core/Log.h"
#include "Renderer2D.h"

using namespace __HGImpl::V1;
using namespace HGCore::Math;

SDL_Texture *Renderer2D::CreateTextureFromFile(const char *pStrFileName) {
    auto pImg = IMG_Load(pStrFileName );
    if( !pImg ) {
        HG_LOG_FAILED( std::string(pStrFileName).append(" -> create texture failed; details:").append( IMG_GetError() ).c_str());
        return nullptr;
    } else {
        HG_LOG_SUCCESS( std::string(pStrFileName).append(" -> create texture ").c_str() );
    }

    auto pTex = SDL_CreateTextureFromSurface( pHandle, pImg );
    if ( pTex == nullptr ) {
        HG_LOG_SDL_ERROR( SDL_LOG_CATEGORY_RENDER, "SDL_CreateTextureFromSurface in Renderer2D::CreateTextureFromFile" );
        return nullptr;
    } else {
        HG_LOG_SUCCESS( "SDL_CreateTextureFromSurface in Renderer2D::CreateTextureFromFile" );
    }

    SDL_FreeSurface( pImg );
    pImg = nullptr;

    umTextures[ pStrFileName ] = pTex;

    return pTex;
}

void Renderer2D::Copy(const GameObject2D *pGameObject, HGRect *pSrcRect, HGRect *pDstRect ) {
    SDL_Rect *pSrc = nullptr, *pDst = nullptr;

    if ( pSrcRect != nullptr ) {
        auto tSrc = SDL_Rect {
            .x = pSrcRect->X,
            .y = pSrcRect->Y,
            .w = static_cast<int>(pSrcRect->W),
            .h = static_cast<int>(pSrcRect->H),
        };
    }
    if ( pDstRect != nullptr ) {
        auto tDst = SDL_Rect{
                .x = pDstRect->X,
                .y = pDstRect->Y,
                .w = static_cast<int>(pDstRect->W),
                .h = static_cast<int>(pDstRect->H),
        };
    }
    SDL_RenderCopy( pHandle, pGameObject->GetTexture(), pSrc, pDst );
}
