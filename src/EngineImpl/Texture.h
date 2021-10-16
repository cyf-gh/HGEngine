#pragma once

#include "../Engine/HGObject.h"
#include "EngineImpl.h"

namespace HGEngine {
namespace V1SDL {

class Texture : public HG::HGObject<Texture> {
private:
	SDL_Texture* pHandle;
	std::string m_strFileName;

public:
	HG::Math::HGSize<int> GetSize();
	SDL_Texture* GetHandle() const { return pHandle; };
	explicit Texture( const char* strTexture, const char* pStrFileName ) 
	: HG::HGObject<Texture>( strTexture ), m_strFileName( pStrFileName ) {
		pHandle = EngineImpl::GetEngine()->GetRenderer2D()->CreateTextureFromFile( pStrFileName );
	}
	~Texture() { 
		SDL_DestroyTexture( pHandle );
	}
};


}}