#include "EngineImpl/EngineImpl.h"
#include "SceneMainGame.h"
#include <string>

using namespace __HGImpl::V1;

int main( int argc, char **argv ) {
    // scenes -> engine -> game objects
    SceneMainGame t("SceneMainGame");

    auto *tEngine = new EngineImpl( argc, argv );
    HG_LOG_INFO( tEngine->GetCurrentScene()->GetName() );
    GameObject2D testImg1FullScreen = GameObject2D("test_full_screen", R"(C:\Users\cyf-m\Pictures\icon.png)");
    testImg1FullScreen.Enable();

    HG_LOG_INFO( t.FindGameObject( "test_full_screen" )->GetName() );

    tEngine->Run();
    HG_SAFE_DEL( tEngine );
    return 0;
}


