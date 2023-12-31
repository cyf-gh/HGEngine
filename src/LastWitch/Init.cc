/// @brief Init�ű� ��ɶ���Ϸ��Դ������
#include <engine/HGEngine.hpp>

HG_SCRIPT_START( SCRIPT_INIT ) {

	HG_ENGINE_ASSET()->SetHomeDirectory( HG::HGDirectory::GetApplicationDirectory().c_str() );
	HG_ENGINE_ASSET()->CreateTexture( "NULL", R"(\Assets\null.png)" );
	HG_ENGINE_ASSET()->CreateTexture( "Chars", R"(\Assets\1.png)" );
	HG_ENGINE_ASSET()->CreateTexture( "Bg", R"(\Assets\2.png)" );
	HG_ENGINE_ASSET()->CreateFont( "Default", R"(\Assets\Minimal.ttf)", 50 );

} HG_SCRIPT_END