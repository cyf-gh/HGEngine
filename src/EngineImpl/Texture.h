#pragma once

#include <Math.hpp>
#include "../Engine/HGObject.h"
#include "EngineImpl.h"

namespace HGEngine {
namespace V1SDL {

class Texture : public HG::HGObject<Texture> {
private:
	SDL_Texture* pHandle;
	std::string m_strFileName;
	HG::Math::HGSize<int> m_tSize;

public:
	SDL_Texture* GetHandle() const { return pHandle; }
	void SetHandle( SDL_Texture* pH ) { pHandle = pH; }
	HG_INLINE void SetAlphaMode( SDL_BlendMode blending = SDL_BLENDMODE_BLEND ) {
		SDL_SetTextureBlendMode( pHandle, blending );
	}
	HG_INLINE void SetAlpha( un8 a ) {
		SDL_SetTextureAlphaMod( pHandle, a );
	}
	explicit Texture( const char* strTexture, const char* pStrFileName )
		: HG::HGObject<Texture>( strTexture ), m_strFileName( pStrFileName ) {
		pHandle = EngineImpl::GetEngine()->GetRenderer2D()->CreateTextureFromFile( pStrFileName );
	}
	explicit Texture( const char* strTexture, SDL_Texture* pt )
		: HG::HGObject<Texture>( strTexture ), m_strFileName(), pHandle( pt ) { }
	const HG::Math::HGSize<int>& GetSize();
	~Texture() {
		SDL_DestroyTexture( pHandle );
	}
};


}
}