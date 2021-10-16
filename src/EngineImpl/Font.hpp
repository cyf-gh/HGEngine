#pragma once
#include <SDL_ttf.h>
#include <Log.h>
#include "../Engine/HGObject.h"

namespace HGEngine {
namespace V1SDL {
class FontImpl : public HG::HGObject<FontImpl> {
protected:
	TTF_Font* pHandle;

public: 
	void* GetHandle() const { return pHandle; }
	FontImpl( const char* strName, const char *file, int ptsize ) : HG::HGObject<FontImpl, HGEngine::IFont>( strName ), pHandle( nullptr ) {
		pHandle = TTF_OpenFont( file, ptsize );
		if ( !pHandle ) {
			HG_LOG_FAILED( "Failed to load font: " );
			HG_LOG_FAILED( TTF_GetError() );
		}
	}
	~FontImpl() {
		TTF_CloseFont( pHandle );
	}
};
}
}