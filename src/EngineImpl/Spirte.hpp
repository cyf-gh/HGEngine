#pragma once

#include <SDL.h>
#include "../engine/HGComponent.h"
#include "EngineImpl.h"
#include "Texture.h"
#include "Asset.h"

namespace HGEngine {
namespace V1SDL {

class Texture;

class Spirte : public HG::HGComponent {
private:	
	Texture* m_pTx;
public:
	HG_COMPONENT_RENDERABLE
	Texture* GetRenderTarget( Renderer2D* pRd ) override { return m_pTx; }

	Texture* GetTexture() const { return m_pTx; }
	void SetTexture( Texture* t ) { m_pTx = t; }
	void SetTexture( const char* strTextureName ) {
		auto pT = EngineImpl::GetEngine()->GetAssetManager()->GetAsset<Texture>( strTextureName );
		if( pT == nullptr ) {
			HG_LOG_WARNNING( std::format( "Texture[{}] does not exsit in SetTexture()", strTextureName ).c_str() );
			return;
		}
		SetTexture( pT );
	}
	explicit Spirte( const char* strName, Texture* pT = nullptr ) : HGComponent( strName ), m_pTx( pT ) { 
		nRenderIndex = HG::HGRenderableComponentSeq::SPRITE;
	}
	explicit Spirte( const char* strName, const char* strTextureName ) : HGComponent( strName ), m_pTx( nullptr ) { 
		nRenderIndex = HG::HGRenderableComponentSeq::SPRITE;
		SetTexture( strTextureName );
	}
	explicit Spirte() : HGComponent(), m_pTx( nullptr ) { };
	virtual ~Spirte() { }
};
}
}