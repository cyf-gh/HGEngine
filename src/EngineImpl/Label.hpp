#pragma once

#include <SDL.h>
#include "../engine/HGComponent.h"
#include "Font.hpp"
#include "EngineImpl.h"
#include "Renderer2D.h"
#include "Asset.h"
#include "Texture.h"

namespace HGEngine {
namespace V1SDL {

class Font;

class Label : public HG::HGComponent {
private:
	SDL_Surface* m_pSf;
	SDL_Texture* m_pTx;
	Texture m_tT;
	Font* m_pFont;

public:
	/// @brief 可以指定文字于 GameObject 中的位置
	HG::Math::HGRect m_tRect;

public:
	HG::Math::HGRect* GetLocalRectOffset() override { return &m_tRect; }
	HG_COMPONENT_RENDERABLE
	Texture* GetRenderTarget( Renderer2D* pRd ) override {
		RenderText2Texture( pRd );
		m_tT.SetHandle( m_pTx );
		return &m_tT; 
	}
	std::string Text;
	SDL_Color tColor;
	Font* GetFont() const { return m_pFont; }
	void SetFont( Font* t ) { m_pFont = t; }
	void SetFont( const char* strFontName ) { 
		auto pT = EngineImpl::GetEngine()->GetAssetManager()->GetAsset<Font>( strFontName );
		if( pT == nullptr ) {
			HG_LOG_WARNNING( std::format( "Texture[{}] does not exsit in SetTexture()", strFontName ).c_str() );
			return;
		}
		SetFont( pT );
	}
	SDL_Texture* GetTexture() const { return m_pTx; }
	/// @brief 输出原始字体大小
	void SetFontSize2Rect() {
		if( m_pTx != nullptr ) {
			m_tRect.SetSize( m_tT.GetSize() );
		}
	}
	/// @brief 
	/// @param pRenderer 
	/// @return 当渲染成功时返回true
	bool RenderText2Texture( Renderer2D* pRenderer ) {
		SDL_FreeSurface( m_pSf );
		m_pSf = nullptr;
		if( m_pTx != nullptr ) {
			SDL_DestroyTexture( m_pTx );
			m_pTx = nullptr;
		}
		m_pSf = TTF_RenderText_Solid( ( TTF_Font* ) m_pFont->GetHandle(), Text.c_str(), tColor );
		if( m_pSf == nullptr ) {
			HG_LOG_FAILED( "Failed to render text: " );
			HG_LOG_FAILED( TTF_GetError() );
		} else {
			auto pRd2D = static_cast< Renderer2D* >( pRenderer );
			m_pTx = SDL_CreateTextureFromSurface( pRd2D->pHandle, m_pSf );
			if( m_pTx != nullptr ) {
				return true;
			} else {
				HG_LOG_FAILEDF( "RenderText2Texture !SDL_CreateTextureFromSurface Details:[{}]", SDL_GetError() );
			}
		}
		return false;
	}
	explicit Label( const char* strName, const char* str,  Font* pT = nullptr ) : HGComponent( strName ), m_pSf( nullptr ), m_pTx( nullptr ), m_pFont( pT ), Text( str ), m_tRect(), m_tT( strName, m_pTx ) {
		nRenderIndex = HG::HGRenderableComponentSeq::LABEL;
	}
	explicit Label( const char* strName, const char* str, const char* strFontName ) : HGComponent( strName ), m_pSf( nullptr ), m_pTx( nullptr ), m_pFont( nullptr ), Text( str ), m_tRect(), m_tT( strName, m_pTx ) {
		nRenderIndex = HG::HGRenderableComponentSeq::LABEL;
		SetFont( strFontName );
	}
	explicit Label() : HGComponent(), m_pSf( nullptr ), m_pTx( nullptr ), m_pFont( nullptr ), m_tRect(), m_tT( "", m_pTx ) { };
	virtual ~Label() {
		if( m_pTx != nullptr ) {
			SDL_DestroyTexture( m_pTx );
		}
		if( m_pSf != nullptr ) {
			SDL_FreeSurface( m_pSf );
		}
	}
};
}
}