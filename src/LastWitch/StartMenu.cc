#include <engine/HGEngine.hpp>

HG_SCRIPT_START( SCRIPT_STARTMENU )

auto* s = HGX::GetSceneWithGUI( "StartMenu" );
auto* canvas = s->GetGUI( "Canvas" );

canvas->OnGUI = HG_EVENT_IMPL {
	auto gui = static_cast< GUI* >( pThis );
	return 0;
};

EngineImpl::GetEngine()->NavigateScene( "StartMenu" );

HG_SCRIPT_END