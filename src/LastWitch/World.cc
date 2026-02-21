#include <engine/HGEngine.hpp>
#include <engineImpl/SceneLoader.h>
#include <engineImpl/Audio.h>

HG_SCRIPT_START( SCRIPT_WORLD ) {

    auto* s = HGX::GetSceneWithGUI( "World" );
    EngineImpl::GetEngine()->NavigateScene( s );

    SceneConfig config = SceneLoader::LoadFromFile( "assets/scenes/world.json" );
    
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

    GameObject* go_MainChar = GameObject::Find( "MainCharacter" );

    HG_EVENT_BIND( s->GetMainCamera(), OnFixedUpdate ) {
        auto _this = HG_EVENT_THIS_GAMEOBJECT;
        auto c_Camera_Tr = _this->GetComponent<Transform>();
        auto go_MainChar = GameObject::Find( "MainCharacter" );
        auto c_MainChar_Tr = go_MainChar->GetComponent<Transform>();
    
        auto go_Bg = GameObject::Find( "Ground" );
        auto c_Bg_Tr = go_Bg->GetComponent<Transform>();

        static HGRect tNewCameraGRect;
        tNewCameraGRect.X = c_MainChar_Tr->tPosition.X - ( c_Camera_Tr->tRect.W / 2 ) + ( c_MainChar_Tr->tRect.W / 2 );
        tNewCameraGRect.Y = c_MainChar_Tr->tPosition.Y - ( c_Camera_Tr->tRect.H / 2 ) + ( c_MainChar_Tr->tRect.H / 2 );
        tNewCameraGRect.H = c_Camera_Tr->tRect.H;
        tNewCameraGRect.W = c_Camera_Tr->tRect.W;

        if( c_Bg_Tr->ToHGRectGlobal().IsInX( tNewCameraGRect ) ) {
            c_Camera_Tr->tPosition.X = tNewCameraGRect.X;
        }
        if( c_Bg_Tr->ToHGRectGlobal().IsInY( tNewCameraGRect ) ) {
            c_Camera_Tr->tPosition.Y = tNewCameraGRect.Y;
        }
        return 0;
    };

    HG_EVENT_BIND( go_MainChar, OnFixedUpdate ) {
        auto _this = HG_EVENT_THIS_GAMEOBJECT;
        auto rb = _this->GetComponent<RigidBodyB2>();
        
        switch( HG_ENGINE_INPUT()->GetDirect() ) {
        case HG::HGInput::UP:
            rb->ApplyForce( b2Vec2( 0, -100.f ) );
            break;
        case HG::HGInput::LEFT:
            rb->ApplyForce( b2Vec2( -50.0f, 0.f ) );
            break;
        case HG::HGInput::RIGHT:
            rb->ApplyForce( b2Vec2( 50.0f, 0.f ) );
            break;
        case HG::HGInput::UPL:
            rb->ApplyForce( b2Vec2( -50.0f, -100.f ) );
            break;
        case HG::HGInput::UPR:
            rb->ApplyForce( b2Vec2( 50.0f, -100.f ) );
            break;
        default:
            break;
        }
        return 0;
    };

} HG_SCRIPT_END
