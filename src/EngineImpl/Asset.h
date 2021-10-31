#pragma once

#include "../Engine/HGObject.h"

namespace HGEngine {
namespace V1SDL {

class Font;
class Texture;

class Asset final {

public:
	/// @brief 创建字体资源
	/// @param strFontName 定义名
	/// @param strFontFilePath 字体文件路径
	/// @param unPtSize 字号
	/// @return 字体句柄
	Font* CreateFont( const char *strFontName, const char *strFontFilePath, const un32 unPtSize );
	/// @brief 创建材质资源
	/// @param strTextureName 
	/// @param strTextureFilePath 
	/// @return 材质句柄
	Texture* CreateTexture( const char* strTextureName, const char* strTextureFilePath );
	/// @brief 获取资源
	template<typename T> 
	HG_INLINE T *GetAsset( const char *strAssetName ) {
		return static_cast<T*>( HG::HGObject<T>::Find( strAssetName ) );
	}
	template<typename T>
	HG_INLINE T *GetDefaultAsset() {
		return static_cast< T* >( HG::HGObject<T>::Find( "default" ) );
	}
	/// @brief 释放资源
	template<typename T>
	HG_INLINE T* ReleaseAsset( const char* strAssetName ) {
		HG_SAFE_DEL( HG::HGObject<T>::Find( strAssetName ) );
	}
	Asset()	 = default;
	~Asset() = default;
};

}
}