#include "EngineImpl.h"
#include "../Engine/HGObject.h"
#include "Asset.h"

using namespace HG;
using namespace HGEngine::V1SDL;

Font* HGEngine::V1SDL::Asset::CreateFont( const char *strFontName, const char *strFontFilePath, const un32 unPtSize ) {
	auto pF = new Font(	strFontName, strFontFilePath, unPtSize );
	return pF;
}
