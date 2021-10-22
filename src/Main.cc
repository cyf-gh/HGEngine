// #define __HG_TEST__

#ifdef __HG_TEST__
#include <utest.h>

UTEST_MAIN()

#else
#include <string>
#include "EngineImpl/EngineImpl.h"
#include "EngineImpl/Scene.h"
using namespace std;
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

