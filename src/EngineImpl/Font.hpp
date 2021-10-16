#pragma once
#include <SDL_ttf.h>
#include <Log.h>
#include "../Engine/HGObject.h"

namespace HGEngine {
namespace V1SDL {
class Font : public HG::HGObject<Font> {
protected:
	TTF_Font* pHandle;
	std::string m_strFileName;

public: 
	void* GetHandle() const { return pHandle; }
	Font( const char* strName, const char *file, int ptsize ) : HG::HGObject<Font>( strName ), pHandle( nullptr ), m_strFileName( file ) {
		pHandle = TTF_OpenFont( file, ptsize );
		if ( !pHandle ) {
			HG_LOG_FAILED( "Failed to load font: " );
			HG_LOG_FAILED( TTF_GetError() );
		}
	}
	~Font() {
		TTF_CloseFont( pHandle );
	}
};
}
}