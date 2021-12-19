#include <string>
#include <Log.h>
#include <engineImpl/EngineImpl.h>
#include <engineImpl/Scene.h>
using namespace std;
using namespace HGEngine::V1SDL;
//
//#pragma data_seg(".CRT$XIU")
//

#undef main

HGMain( int argc, char** argv ) {
	// scenes -> engine -> game objects
	auto* pScene = new Scene( "Default" );
	auto* tEngine = new EngineImpl( argc, argv );
	HG_SCRIPT_BINDALL();

	tEngine->Run();
	HG_SAFE_DEL( pScene );
	HG_SAFE_DEL( tEngine );
	return 0;
}

