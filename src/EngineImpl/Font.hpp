#pragma once
#include <SDL_ttf.h>
#include "Log.hpp"
#include "../Engine/HGObject.h"

namespace HGEngine {
namespace V1SDL {
class Font : public HG::HGObject<Font> {
protected:
	TTF_Font* pH;
	std::string m_strFileName;

public: 
	void* GetHandle() const { return pH; }
	Font( const char* strName, const char *file, int ptsize ) : HG::HGObject<Font>( strName ), pH( nullptr ), m_strFileName( file ) {
		pH = TTF_OpenFont( file, ptsize );
		if ( !pH ) {
			HG_LOG_FAILED( "Failed to load font: " );
			HG_LOG_FAILED( TTF_GetError() );
		}
	}
	~Font() {
		TTF_CloseFont( pH );
	}
};
}
}