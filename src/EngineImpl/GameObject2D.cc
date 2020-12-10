//
// Created by cyf-m on 2020/12/9.
//

#include <SDL_image.h>
#include "GameObject2D.h"
#include "Renderer2D.h"
#include "EngineImpl.h"
#include "../Core/Log.h"

using namespace __HGImpl::V1;

GameObject2D::GameObject2D(const char *strObjectName, const char *strFileName)
: GameObject( strObjectName ), pTexture( nullptr ), strFileName( strFileName ) {
    if ( strlen ( strFileName ) != 0) {
        pTexture = EngineImpl::GetEngine()->GetRenderer2D()->CreateTextureFromFile(strFileName);
    }
}

GameObject2D::~GameObject2D() {
    if ( pTexture != nullptr ) {
        SDL_DestroyTexture( pTexture ) ;
        EngineImpl::GetEngine()->GetRenderer2D()->umTextures[strFileName] = nullptr;
        HG_SAFE_DEL( pTexture );
    }
}

void GameObject2D::Update(void *pEvent) {

}

void GameObject2D::Render(HGCore::Renderer *pRenderer) {
    auto pRd2D = dynamic_cast<Renderer2D*>( pRenderer );
    pRd2D->Copy( this );
}

void GameObject2D::OnEnable() {

}

void GameObject2D::OnDisable() {

}

void GameObject2D::OnAttach() {

}
