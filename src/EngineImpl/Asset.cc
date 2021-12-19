#include "EngineImpl.h"
#include "../Engine/HGObject.h"
#include "Font.hpp"
#include "Texture.h"
#include "Asset.h"

using namespace HG;
using namespace HGEngine::V1SDL;

Font* HGEngine::V1SDL::Asset::CreateFont( const char *strFontName, const char *strFontFilePath, const un32 unPtSize ) {
	auto pF = new Font(	strFontName, ( m_strHomeDir + strFontFilePath ).c_str(), unPtSize );
	return pF;
}

Texture* HGEngine::V1SDL::Asset::CreateTexture( const char* strTextureName, const char* strTextureFilePath ) {
	auto pF = new Texture( strTextureName, ( m_strHomeDir + strTextureFilePath ).c_str() );
	return pF;
}
