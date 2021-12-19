/// @brief Init脚本 完成对游戏资源的载入
#include <engine/HGEngine.hpp>

HG_SCRIPT_START( SCRIPT_INIT )

HG_ENGINE_ASSET()->SetHomeDirectory( HG::HGDirectory::GetApplicationDirectory().c_str() );
HG_ENGINE_ASSET()->CreateTexture( "test", R"(\Assets\1.png)" );
HG_ENGINE_ASSET()->CreateFont( "f", R"(\Assets\Minimal.ttf)", 50 );

HG_SCRIPT_END