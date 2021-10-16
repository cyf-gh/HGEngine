#pragma once

#include "Font.hpp"

namespace HGEngine {
namespace V1SDL {

class Asset final {

public:
	/// @brief 创建字体资源
	/// @param strFontName 定义名
	/// @param strFontFilePath 字体文件路径
	/// @param unPtSize 字号
	/// @return 字体句柄
	Font* CreateFont( const char *strFontName, const char *strFontFilePath, const un32 unPtSize );

	/// @brief 获取资源
	template<typename T> 
	HG_INLINE HGObject<T> *GetAsset( const char *strAssetName ) {
		return HGObject<T>::Find( strAssetName );
	}

	Asset()	 = default;
	~Asset() = default;
};

}
}