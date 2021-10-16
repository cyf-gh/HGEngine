#include "EngineImpl/EngineImpl.h"
#include <Math.hpp>
#include "Core/Test.hpp"
#include "EngineImpl/Scene.h"
#include <string>
/*
#define __HG_TEST__
*/

using namespace std;
#ifdef __HG_TEST__

#undef main
int main() {
	HG_TEST_START( "rect" );

	HG::Math::HGRect rect {
		.X = 0,
		.Y = 0,
		.H = 600,
		.W = 800
	};

	auto center = rect.GetCenter();
	HG_TEST_ASSERT_TRUE( center.X == 400 && center.Y == 300, "HG::Math::HGRect.GetCenter()" );
	HG::Math::HGShape<float> tRect;
	rect.ToShape<float>(tRect);
	tRect.Rotate( 90, rect.GetCenter().ToVec2<float>() );
	return 0;
}

#else

using namespace HGEngine::V1SDL;

#undef main
HGMain( int argc, char** argv ) {
	// scenes -> engine -> game objects
	auto* pScene = new Scene( "SceneMainGame" );
	auto* tEngine = new EngineImpl( argc, argv );
	HG_SCRIPT_BINDALL();

	HG_LOG_INFO( pScene->FindGameObject( "test_full_screen" )->GetName() );
	HG_LOG_INFO( pScene->FindGameObject( "test_fps" )->GetName() );

	tEngine->Run();
	HG_SAFE_DEL( pScene );
	HG_SAFE_DEL( tEngine );
	return 0;
}
#endif
