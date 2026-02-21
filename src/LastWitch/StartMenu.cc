#include <engine/HGEngine.hpp>
#include <engineImpl/SceneLoader.h>
#include <engineImpl/Audio.h>

HG_SCRIPT_START( SCRIPT_STARTMENU ) {

    auto* s = HGX::GetSceneWithGUI( "StartMenu" );
    EngineImpl::GetEngine()->NavigateScene( s );

    SceneConfig config = SceneLoader::LoadFromFile( "assets/scenes/startmenu.json" );
    
    if( !config.name.empty() ) {
        auto* pCamera = s->GetMainCamera();
        auto c_Camera_Tr = pCamera->GetComponent<Transform>();
        c_Camera_Tr->tPosition.X = (float)config.cameraX;
        c_Camera_Tr->tPosition.Y = (float)config.cameraY;
        c_Camera_Tr->tRect.W = config.cameraWidth;
        c_Camera_Tr->tRect.H = config.cameraHeight;

        SceneLoader::CreateSceneObjects( s, config.objects );
        SceneLoader::CreateSceneObjects( s, config.uiObjects );
    }

    //AudioSystem* audio = AudioSystem::GetInstance();
    //if( audio->Initialize() ) {
    //    HG_LOG_INFO( "Audio system initialized - Start Menu" );
    //}

} HG_SCRIPT_END
