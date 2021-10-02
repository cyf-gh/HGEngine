#include "EngineImpl/EngineImpl.h"
#include "Core/Math.h"
#include "Core/Test.hpp"
#include "EngineImpl/Scene.h"
#include <string>
/*
#define __HG_TEST__
*/


#ifdef __HG_TEST__

#undef main
int main() {
	HG_TEST_START( "rect" );

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

using namespace __HGImpl::V1SDL;

#undef main
int main( int argc, char** argv ) {
	// scenes -> engine -> game objects
	auto* pScene = new Scene( "SceneMainGame" );

	pScene->OnFixedUpdate = HG_EVENT_ONUPDATE() {
		switch( HG_EVENT_ONUPDATE_EVENT->type ) {
		case SDL_KEYDOWN:
		//HG_LOG_INFO( SDL_GetKeyName( HG_EVENT_ONUPDATE_EVENT->key.keysym.sym ) );
		//HG_LOG_INFO( std::to_string( EngineImpl::GetEngine()->GetMainLoop().GetCurrentFps() ).append( " <- main loop fps" ).c_str() );
		//HG_LOG_INFO( std::to_string( EngineImpl::GetEngine()->GetRenderLoop().GetCurrentFps() ).append( " <- render loop fps" ).c_str() );
		//HG_LOG_INFO( std::to_string( EngineImpl::GetEngine()->GetUpdateLoop().GetCurrentFps() ).append( " <- update loop fps" ).c_str() );
		break;
		case SDL_QUIT:
		HG_LOG_INFO( "bye!!!" );
		EngineImpl::GetEngine()->Exit();
		return 0;
		break;
		}
	};
	pScene->OnRender = HG_EVENT_ONRENDER() {
		static int i = 0;
		GameObjectText* tFpsText = static_cast< GameObjectText* >( GameObject::Find( "test_fps" ) );
		if( tFpsText != nullptr ) {
			auto str = std::to_string( EngineImpl::GetEngine()->GetRenderLoop().GetCurrentFps() );
			str.resize( 3 );
			str += + " | " + std::to_string(i);
			i++;
			tFpsText->Text = str;
		}
		return 0;
	};

	auto* tEngine = new EngineImpl( argc, argv );
	HG_LOG_INFO( tEngine->GetCurrentScene()->GetName() );

	GameObject2D* pImgTest = new GameObject2D( "test_full_screen", R"(C:\Users\cyf-m\Pictures\icon.png)" );
	GameObjectText* pText = new GameObjectText( "test_fps", new Font( "font1", R"(C:\Users\cyf-m\Documents\Minimal.ttf)", 24 ), "0" );
	auto df = pText->GetComponent<Transform>();

	df->tPosition.X = 5;
	df->tPosition.Y = 5;
	df->tRect.H = 30;
	df->tRect.W = 200;

	pText->OnFixedUpdate = HG_EVENT_ONUPDATE() {
		auto _this = HG_EVENT_THIS_GAMEOBJECT;
		auto df = _this->GetComponent<Transform>();
		auto timedelta = HG_ENGINE_TIMEDELTA;
		// df->f64Angle += 10 * HG_ENGINE_TIMEDELTA;

		switch( HG_EVENT_ONUPDATE_EVENT->type ) {
		case SDL_KEYDOWN:
		HG_EVENT_ONUPDATE_ISKEY( SDLK_UP ) {
			df->tPosition.Y -= 100 * HG_ENGINE_TIMEDELTA;
		}
		HG_EVENT_ONUPDATE_ISKEY( SDLK_DOWN ) {
			df->tPosition.Y += 100 * HG_ENGINE_TIMEDELTA;
		}
		HG_EVENT_ONUPDATE_ISKEY( SDLK_LEFT ) {
			df->tPosition.X -= 100 * HG_ENGINE_TIMEDELTA;
		}
		HG_EVENT_ONUPDATE_ISKEY( SDLK_RIGHT ) {
			df->tPosition.X += 100 * HG_ENGINE_TIMEDELTA;
		}
		break;
		}
		df->ResetRotateCenter();
		return 0;
	};

	pImgTest->OnFixedUpdate = HG_EVENT_ONUPDATE() {
		auto _this = HG_EVENT_THIS_GAMEOBJECT;
		auto df = _this->GetComponent<Transform>();
		df->f64Angle += 10 * HG_ENGINE_TIMEDELTA;
		df->tRotateCenter.X = 400;
		df->tRotateCenter.Y = 300;
		return 0;
	};
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
