#include "../src/Engine/HGEvent.h"
#include "../src/EngineImpl/EngineImpl.h"
#include "../src/Core/Math.h"
#include "../src/Core/Test.hpp"
#include "../src/EngineImpl/Scene.h"
#include "../src/EngineImpl/Collision.h"
#include <string>
using namespace __HGImpl::V1SDL;

HG_SCRIPT_START( SCRIPT1 )

HG_LOG_INFO( EngineImpl::GetEngine()->GetCurrentScene()->GetName() );
Camera* pCamera = new Camera( "camera" );
pCamera->Enable();
pCamera->SetCameraSizeToRendererSize();
EngineImpl::GetEngine()->GetCurrentScene()->SetMainCamera( pCamera );
GameObject2D* pImgTest = new GameObject2D( "test_full_screen", R"(C:\Users\cyf-m\Pictures\icon.png)" );
GameObject2D* pImgTestColMain = new GameObject2D( "test_main", R"(C:\Users\cyf-m\Pictures\icon.png)" );
GameObject2D* pImgTestCol2 = new GameObject2D( "test_main_2", R"(C:\Users\cyf-m\Pictures\icon.png)" );

auto bcMain = static_cast< BoundingCollision* >( pImgTestColMain->AddComponent( new BoundingCollision( "Collision" ) ) );
auto bc2 = static_cast< BoundingCollision* >( pImgTestCol2->AddComponent( new BoundingCollision( "Collision" ) ) );

auto dfMain = pImgTestColMain->GetComponent<Transform>();
dfMain->tPosition.X = 300;
dfMain->tPosition.Y = 400;
dfMain->tRect.W = 100;
dfMain->tRect.H = 100;
dfMain = pImgTestCol2->GetComponent<Transform>();
dfMain->tPosition.X = 100;
dfMain->tPosition.Y = 100;
dfMain->tRect.W = 300;
dfMain->tRect.H = 300;
pImgTestColMain->Enable();
pImgTestCol2->Enable();
GameObjectText* pText = new GameObjectText( "test_fps", new Font( "font1", R"(C:\Users\cyf-m\Documents\Minimal.ttf)", 50 ), "0" );
GameObjectText* pText2 = new GameObjectText( "test_texts", new Font( "font2", R"(C:\Users\cyf-m\Documents\Minimal.ttf)", 50 ), "0" );

auto df = pText->GetComponent<Transform>();

df->tPosition.X = 5;
df->tPosition.Y = 5;
df->tRect.H = 30;
df->tRect.W = 200;

df = pText2->GetComponent<Transform>();
df->tPosition.X = 5;
df->tPosition.Y = 300;
df->tRect.H = 30;
df->tRect.W = 200;
pText2->Text = "hello";

auto df3 = pCamera->GetComponent<Transform>();
df3->tRect.H = 600;
df3->tRect.W = 800;

HG_EVENT_BIND( pCamera, OnFixedUpdate ) {
	auto _this = HG_EVENT_THIS_GAMEOBJECT;
	auto df = _this->GetComponent<Transform>();
	switch( HG_EVENT_ONUPDATE_EVENT->type ) {
	case SDL_KEYDOWN:
	HG_EVENT_ONUPDATE_ISKEY( SDLK_UP ) {
		df->tPosition.Y -= 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_DOWN ) {
		df->tPosition.Y += 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_LEFT ) {
		df->tPosition.X -= 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_RIGHT ) {
		df->tPosition.X += 200 * HG_ENGINE_TIMEDELTA;
	}
	break;
	case SDL_QUIT:
	HG_LOG_INFO( "bye!!!" );
	EngineImpl::GetEngine()->Exit();
	return 0;
	break;
	}
	return 0;
};
HG_EVENT_BIND( pText, OnFixedUpdate ) {
	auto _this = HG_EVENT_THIS_GAMEOBJECT;
	auto df = _this->GetComponent<Transform>();
	switch( HG_EVENT_ONUPDATE_EVENT->type ) {
	case SDL_KEYDOWN:
	HG_EVENT_ONUPDATE_ISKEY( SDLK_UP ) {
		df->tPosition.Y -= 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_DOWN ) {
		df->tPosition.Y += 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_LEFT ) {
		df->tPosition.X -= 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_RIGHT ) {
		df->tPosition.X += 200 * HG_ENGINE_TIMEDELTA;
	}
	break;
	}
	return 0;
};
HG_EVENT_BIND( pText2, OnFixedUpdate ) {
	auto _this = HG_EVENT_THIS_GAMEOBJECT;
	auto df = _this->GetComponent<Transform>();
	switch( HG_EVENT_ONUPDATE_EVENT->type ) {
	case SDL_KEYDOWN:
	HG_EVENT_ONUPDATE_ISKEY( SDLK_UP ) {
		df->tPosition.Y -= 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_DOWN ) {
		df->tPosition.Y += 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_LEFT ) {
		df->tPosition.X -= 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_RIGHT ) {
		df->tPosition.X += 200 * HG_ENGINE_TIMEDELTA;
	}
	break;
	}
	return 0;
};
HG_EVENT_BIND( pImgTestColMain, OnFixedUpdate ) {
	auto _this = HG_EVENT_THIS_GAMEOBJECT;
	auto df = _this->GetComponent<Transform>();
	switch( HG_EVENT_ONUPDATE_EVENT->type ) {
	case SDL_KEYDOWN:
	HG_EVENT_ONUPDATE_ISKEY( SDLK_UP ) {
		df->tPosition.Y -= 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_DOWN ) {
		df->tPosition.Y += 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_LEFT ) {
		df->tPosition.X -= 200 * HG_ENGINE_TIMEDELTA;
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_RIGHT ) {
		df->tPosition.X += 200 * HG_ENGINE_TIMEDELTA;
	}
	break;
	}
	return 0;
};
HG_EVENT_BIND( pText, OnRender ) {
	static int i = 0;
	GameObjectText* tFpsText = static_cast< GameObjectText* >( GameObject::Find( "test_fps" ) );
	if( tFpsText != nullptr ) {
		auto str = std::to_string( EngineImpl::GetEngine()->GetRenderLoop().GetCurrentFps() );
		str.resize( 3 );
		str += +" | " + std::to_string( i );
		i++;
		tFpsText->Text = str;
	}
	return 0;
};

static int coll = 0;

HG_EVENT_BIND( pImgTestColMain, OnCollisionStay ) {
	auto t = static_cast< GameObjectText* >( HG_ENGINE_FIND_GAMEOBJECT( "test_texts" ) );
	coll++;
	t->Text = "OnCollisionStay | " + std::to_string(coll);
	return 0;
};
HG_EVENT_BIND( pImgTestColMain, OnCollisionExit ) {
	coll= 0;
	auto t = static_cast< GameObjectText* >( HG_ENGINE_FIND_GAMEOBJECT( "test_texts" ) );
	t->Text = "OnCollisionExit | " + std::to_string(coll);
	return 0;
};

auto df2 = pImgTest->GetComponent<Transform>();
df2->tPosition.X = 50;
df2->tPosition.Y = 50;
df2->tRect.W = 100;
df2->tRect.H = 100;

HG_EVENT_BIND( pImgTest, OnFixedUpdate ) {
	auto _this = HG_EVENT_THIS_GAMEOBJECT;
	auto df = _this->GetComponent<Transform>();
	df->f64Angle += 40 * HG_ENGINE_TIMEDELTA;

	df->ResetRotateCenter();

	HG::Math::HGShape<float> s;
	df->GetRotatedRectGlobal( s );
	return 0;
};
pImgTest->Enable();
pText->Enable();
pText2->Enable();
HG_SCRIPT_END