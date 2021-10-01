#pragma once
#include <SDL_ttf.h>
#include "../Core/Log.h"
#include "../Engine/HGObject.h"

namespace __HGImpl {
namespace V1SDL {
class Font : public HG::V1SDL::HGObject<Font> {
public: 
	TTF_Font* pHandle;
	Font( const char* strName, const char *file, int ptsize ) : HG::V1SDL::HGObject<Font>( strName ), pHandle( nullptr ) {
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