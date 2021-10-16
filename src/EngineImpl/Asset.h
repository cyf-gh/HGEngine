#pragma once

#include "Font.hpp"

namespace HGEngine {
namespace V1SDL {

class Asset final {

public:
	/// @brief ����������Դ
	/// @param strFontName ������
	/// @param strFontFilePath �����ļ�·��
	/// @param unPtSize �ֺ�
	/// @return ������
	Font* CreateFont( const char *strFontName, const char *strFontFilePath, const un32 unPtSize );

	/// @brief ��ȡ��Դ
	template<typename T> 
	HG_INLINE HGObject<T> *GetAsset( const char *strAssetName ) {
		return HGObject<T>::Find( strAssetName );
	}

	Asset()	 = default;
	~Asset() = default;
};

}
}