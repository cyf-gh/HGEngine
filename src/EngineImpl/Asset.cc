#include "EngineImpl.h"
#include "../Engine/HGObject.h"
#include "Font.hpp"
#include "Texture.h"
#include "Asset.h"

using namespace HG;
using namespace HGEngine::V1SDL;

Font* HGEngine::V1SDL::Asset::CreateFont( const char *strFontName, const char *strFontFilePath, const un32 unPtSize ) {
	bool fileExists = std::filesystem::exists( m_strHomeDir + strFontFilePath );
	auto pF = new Font(	strFontName, ( m_strHomeDir + strFontFilePath ).c_str(), unPtSize );
	HG_LOG_INFO( std::format( "Font[{}] creating...", strFontName ).c_str() );
	return pF;
}

Texture* HGEngine::V1SDL::Asset::CreateTexture( const char* strTextureName, const char* strTextureFilePath ) {
	Texture* pF = nullptr;
	bool fileExists = std::filesystem::exists( m_strHomeDir + strTextureFilePath );
	HG_LOG_INFO( std::format( "Texture[{}] creating...", strTextureName ).c_str() );
	if( !fileExists ) {
		HG_LOG_FAILED( std::format( "Texture[{}]({}) does not exist. Using null texture", strTextureName, strTextureFilePath ).c_str() );
	}
	pF = new Texture( strTextureName, ( m_strHomeDir + ( fileExists ? strTextureFilePath : R"(\Assets\null.png)" ) ).c_str() );

	return pF;
}
