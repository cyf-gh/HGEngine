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
    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);

	auto* pScene = new Scene( "Default" );
	auto* tEngine = new EngineImpl( argc, argv );
	// HG_SCRIPT_BINDALL();

	tEngine->Render();
	HG_SAFE_DEL( pScene );
	HG_SAFE_DEL( tEngine );
	return 0;
}

