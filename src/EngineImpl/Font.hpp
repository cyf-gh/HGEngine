#pragma once
#include <SDL_ttf.h>
#include "../Core/Log.h"

namespace __HGImpl {
namespace V1 {
class Font : public Object<Font> {
public: 
	TTF_Font* pHandle;
	Font( const char* strName, const char *file, int ptsize ) : Object<Font>( strName ), pHandle( nullptr ) {
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