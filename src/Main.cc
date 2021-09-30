#include "EngineImpl/EngineImpl.h"
#include "SceneMainGame.h"
#include "Core/Math.h"
#include "Core/Test.hpp"
#include <string>
/*
#define __HG_TEST__
*/


#ifdef __HG_TEST__

#undef main
int main() {
    HG_TEST_START("rect");

    HGCore::Math::HGRect rect {
        .X = 0,
        .Y = 0,
        .H = 600,
        .W = 800
    };

    auto center = rect.GetCenter();
    HG_TEST_ASSERT_TRUE( center.X == 400 && center.Y == 300, "HGCore::Math::HGRect.GetCenter()" );

    return 0;
}

#else

using namespace __HGImpl::V1;

#undef main
int main( int argc, char **argv ) {
    // scenes -> engine -> game objects
    auto *pScene = new SceneMainGame("SceneMainGame");

    auto *tEngine = new EngineImpl( argc, argv );
    HG_LOG_INFO( tEngine->GetCurrentScene()->GetName() );

    GameObject2D* pImgTest = new GameObject2D( "test_full_screen", R"(C:\Users\cyf-m\Pictures\icon.png)" );
    GameObjectText* pText = new GameObjectText( "test_fps", new Font( "font1", R"(C:\Users\cyf-m\Documents\Minimal.ttf)", 24 ), "0" );
    pText->tDestFigure.tPosition.X = 5;
    pText->tDestFigure.tPosition.Y = 5;
    pText->tDestFigure.tRect.H = 30;
    pText->tDestFigure.tRect.W = 200;
    pImgTest->Enable();
    pText->Enable();

    HG_LOG_INFO( pScene->FindGameObject( "test_full_screen" )->GetName() );
    HG_LOG_INFO( pScene->FindGameObject( "test_fps" )->GetName() );

    tEngine->Run();
    HG_SAFE_DEL( pScene );
    HG_SAFE_DEL( tEngine );
    return 0;
}
#endif
