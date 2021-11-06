#include <string>
#include <filesystem>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <Math.hpp>
#include <core/Test.hpp>
#include <engine/HGEvent.hpp>
#include <engineImpl/EngineImpl.h>
#include <engineImpl/Scene.h>
#include <engineImpl/Collision.h>
#include <engineImpl/RigidBody.h>
#include <engineImpl/Animation.h>
#include <engineImpl/EngineObjectSerilization.hpp>
#include <engineImpl/Asset.h>
#include <engineImpl/Timer.hpp>
#include <engineImpl/Label.hpp>
#include <engineImpl/Spirte.hpp>
#include <engineImpl/GUI.hpp>
#include <engineImpl/Geometry.hpp>


using namespace HGEngine::V1SDL;
using namespace HG::Math;
using namespace std;

template<class T>
string CheckMarshal(T* t, const char* str = "def") {
	rapidjson::StringBuffer strBuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer( strBuf );
	writer.StartObject();
	HG::Serialization::Marshal( *t, str, writer );
	writer.EndObject();

	string data = strBuf.GetString();
	cout << data << endl;
	return data;
	//rapidjson::Document doc;
	//doc.Parse( data.c_str() );
	//if( doc.HasParseError() ) {
	//	printf( "parseÊ§°Ü:%d\n", doc.GetParseError() );
	//}
	//T ttt;
	//HG::Serialization::Unmarshal( ttt, "unmarshal", doc["123"], doc );
}

HG_SCRIPT_START( SCRIPT1 )

HG_LOG_INFO( EngineImpl::GetEngine()->GetCurrentScene()->GetName() );

EngineImpl::GetEngine()->GetCurrentScene()->CreateGUI( "test_gui", true );

EngineImpl::GetEngine()->GetCurrentScene()->GetGUI( "test_gui" )->OnGUI = HG_EVENT_IMPL {
	auto gui = static_cast<GUI*>( pThis );
	gui->strFontName = "f";

	if ( gui->Button( "Button", HGRect( 0, 0, 30, 100 ) ) ) {
	HG_LOG_INFOF( "Y:{}", EngineImpl::GetEngine()->GetInput()->tGlobalMousePos.Y );
		HG_LOG_INFO( "Button Clicked" );
	}
	return 0;
};

Camera* pCamera = new Camera( "camera" );
pCamera->Enable();
pCamera->SetCameraSizeToRendererSize();
EngineImpl::GetEngine()->GetCurrentScene()->SetMainCamera( pCamera );

EngineImpl::GetEngine()->GetAssetManager()->CreateTexture( "test", R"(C:\Users\cyf-desktop\Pictures\1.png)" );
EngineImpl::GetEngine()->GetAssetManager()->CreateFont( "f", R"(C:\Users\cyf-desktop\Documents\Minimal.ttf)", 50 );
auto ptest = EngineImpl::GetEngine()->GetAssetManager()->GetAsset<Texture>("test");

GameObject* pImgTest = new GameObject( "test_full_screen" );
GameObject* pImgTestColMain = new GameObject( "test_main" );
GameObject* pImgTestCol2 = new GameObject( "test_main_2" );

auto ppps = new Spirte( "Sprite", "test" );

pImgTest->AddComponent( ppps );
pImgTestColMain->AddComponent( ppps );
pImgTestCol2->AddComponent( ppps );

auto t = static_cast< Timer* >( pImgTest->AddComponent( new Timer() ) );

t->DelayStart( 2, 1, 10, 
	HG_EVENT_IMPL {
		HG_LOG_SUCCESS( "Timer Interval!!!!" ); return 0;
	}, HG_EVENT_IMPL {
		HG_LOG_SUCCESS( "Timer Complete!!!!" ); return 0;
	}, HG_EVENT_IMPL {
		HG_LOG_SUCCESS( "Timer Start!!!!" ); return 0;
	} );

auto bcMain = static_cast< BoxCollision* >( pImgTestColMain->AddComponent( new BoxCollision( "Collision" ) ) );
auto rb = static_cast< RigidBody* >( pImgTestColMain->AddComponent( new RigidBody( "RigidBody" ) ) );
auto bc2 = static_cast< BoxCollision* >( pImgTestCol2->AddComponent( new BoxCollision( "Collision" ) ) );
auto rb2 = static_cast< RigidBody* >( pImgTestCol2->AddComponent( new RigidBody( "RigidBody" ) ) );
HGSize<un32> s;
s.W = 48;
s.H = 48;
auto an = static_cast<Animator2D*>(pImgTestColMain->AddComponent(new Animator2D("Animator", s, 3, 4, 1, 0.4f, true)));


CheckMarshal( an );

rb->LinearDrag = 50.0f;
rb->Velocity.X = 0.f;
rb->Velocity.Y = 0.f;
rb->GravityDrag = 300.f;

rb2->IsFrozen = true;

auto dfMain = pImgTestColMain->GetComponent<Transform>();
dfMain->tPosition.X = 200;
dfMain->tPosition.Y = 400;
dfMain->tRect.W = 50;
dfMain->tRect.H = 50;
dfMain = pImgTestCol2->GetComponent<Transform>();
dfMain->tPosition.X = -100;
dfMain->tPosition.Y = 500;
dfMain->tRect.W = 1000;
dfMain->tRect.H = 300;


rapidjson::StringBuffer strBuf;
rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);

////////////////////////////////////////////////////////////////////////////

writer.StartObject();
HG::Serialization::Marshal( *dfMain, "123", writer );
writer.EndObject();

string data = strBuf.GetString();
cout << data << endl;

rapidjson::Document doc;
doc.Parse( data.c_str() );
if (doc.HasParseError())
{
	printf("parseÊ§°Ü:%d\n", doc.GetParseError());
}
Transform ttt;
HG::Serialization::Unmarshal( ttt, "unmarshal", doc["123"], doc );

////////////////////////////////////////////////////////////////////////////

vector<string> a = vector<string>();
a.push_back("123");a.push_back("123");a.push_back("123");a.push_back("123");a.push_back("123");
rapidjson::StringBuffer strBuf2;

rapidjson::Writer<rapidjson::StringBuffer> writer2(strBuf2);
writer2.StartObject();
HG::Serialization::Marshal( a, "123", writer2 );
writer2.EndObject();

data = strBuf2.GetString();
cout << data << endl;


pImgTestColMain->Enable();
pImgTestCol2->Enable();
auto ptext = EngineImpl::GetEngine()->GetAssetManager()->GetAsset<Font>( "f" );
auto pText = new GameObject( "test_fps" );
auto pText2 = new GameObject( "test_texts" );
pText->AddComponent( new Label( "Label", "0", "f" ) );
pText2->AddComponent( new Label( "Label", "0", "f" ) );

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


auto df3 = pCamera->GetComponent<Transform>();
df3->tPosition.X = 0;
df3->tPosition.Y = 0;

CheckMarshal<GameObject>( pText );
rapidjson::Document doc2;
auto j = CheckMarshal<GameObject>( pImgTestColMain, "Obj" );
auto pg = GameObjectFactory::CreateByJson<GameObject>( j, false, "test_json_main" );

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


HG_EVENT_BIND( pImgTestColMain, OnRender ) {
	static f32 r = 300.f;
	r = r <= 0 ? 300.f : --r;
	RdCircle<f32> cccc;
	SDL_SetRenderDrawColor( HG_ENGINE_RENDERER2D->pHandle, 0,0,0,255);
	cccc.Radius = r;
	cccc.tCenter.X = 400;
	cccc.tCenter.Y = 100;

	for( size_t i = 0; i < 20; i++ ) {
		cccc.tCenter.X -= 30;
		cccc.Draw( HG_ENGINE_RENDERER2D );
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
	auto rb = _this->GetComponent<RigidBody>();
	auto an = _this->GetComponent<Animator2D>();
	an->IsIdle = false;
	HG::Math::HGVec2<float> a;
	a.X = 200;
	switch( HG_EVENT_ONUPDATE_EVENT->type ) {
	case SDL_KEYDOWN:
	HG_EVENT_ONUPDATE_ISKEY( SDLK_SPACE ) {
		if ( rb->Velocity.Y <= 10 ) {
			rb->MovePosition( rb->Velocity.X, -200 );
		}
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_DOWN ) {
		rb->MovePosition( rb->Velocity.X, 200 );
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_LEFT ) {
		rb->MovePosition( -100, rb->Velocity.Y );
		an->Row = 1;
		an->ForceSetFrame();
	}
	HG_EVENT_ONUPDATE_ISKEY( SDLK_RIGHT ) {
		rb->MovePosition( 100, rb->Velocity.Y );
		an->Row = 2;
		an->ForceSetFrame();
	}
	break;
	case SDL_KEYUP:
	break;

	}
	if ( HG::Math::Abs( rb->Velocity.X ) <= 5  )
	{
		an->IsIdle = true;
	}
	if (an != nullptr) {
		an->Play(HG_ENGINE_TIMEDELTA);
	}
	return 0;
};
HG_EVENT_BIND( pText, OnRender ) {
	static int i = 0;
	auto tFpsText = GameObject::Find( "test_fps" );
	if( tFpsText != nullptr ) {
		auto lb = tFpsText->GetComponent<Label>();
		auto str = std::to_string( EngineImpl::GetEngine()->GetRenderLoop().GetCurrentFps() );
		str.resize( 3 );
		str += +" | " + std::to_string( i );
		i++;
		lb->Text = str;
	}
	return 0;
};

static int coll = 0;

HG_EVENT_BIND( pImgTestColMain, OnCollisionStay ) {
	auto t = HG_ENGINE_FIND_GAMEOBJECT( "test_texts" )->GetComponent<Label>();
	coll++;
	t->Text = "OnCollisionStay | " + std::to_string(coll);
	return 0;
};
HG_EVENT_BIND( pImgTestColMain, OnCollisionExit ) {
	coll= 0;
	auto t = ( HG_ENGINE_FIND_GAMEOBJECT( "test_texts" )->GetComponent<Label>() );
	t->Text = "OnCollisionExit | " + std::to_string(coll);
	return 0;
};

auto df2 = pImgTest->GetComponent<Transform>();
df2->tPosition.X = 50;
df2->tPosition.Y = 50;
df2->tRect.W = 100;
df2->tRect.H = 100;
df2->ResetRotateCenter();

HG_EVENT_BIND( pImgTest, OnFixedUpdate ) {
	auto _this = HG_EVENT_THIS_GAMEOBJECT;
	auto df = _this->GetComponent<Transform>();
	// df->f64Angle += 40 * HG_ENGINE_TIMEDELTA;


	HG::Math::HGShape<float> s;
	df->GetRotatedRectGlobal( s );
	return 0;
};
pImgTest->Enable();
pText->Enable();
pText2->Enable();


HG_SCRIPT_END