#pragma once

#include "../Engine/HGObject.h"

namespace HGEngine {
namespace V1SDL {

class Font;
class Texture;

class Asset final {

public:
	/// @brief ����������Դ
	/// @param strFontName ������
	/// @param strFontFilePath �����ļ�·��
	/// @param unPtSize �ֺ�
	/// @return ������
	Font* CreateFont( const char *strFontName, const char *strFontFilePath, const un32 unPtSize );
	/// @brief ����������Դ
	/// @param strTextureName 
	/// @param strTextureFilePath 
	/// @return ���ʾ��
	Texture* CreateTexture( const char* strTextureName, const char* strTextureFilePath );
	/// @brief ��ȡ��Դ
	template<typename T> 
	HG_INLINE T *GetAsset( const char *strAssetName ) {
		return static_cast<T*>( HG::HGObject<T>::Find( strAssetName ) );
	}
	template<typename T>
	HG_INLINE T *GetDefaultAsset() {
		return static_cast< T* >( HG::HGObject<T>::Find( "default" ) );
	}
	/// @brief �ͷ���Դ
	template<typename T>
	HG_INLINE T* ReleaseAsset( const char* strAssetName ) {
		HG_SAFE_DEL( HG::HGObject<T>::Find( strAssetName ) );
	}
	Asset()	 = default;
	~Asset() = default;
};

}
}