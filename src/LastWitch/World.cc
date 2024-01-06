#include <engine/HGEngine.hpp>

using namespace std;
void CreateC() {

	for( size_t i = 4; i < 50; i++ ) {
		GameObject* go_MainChar2 = new GameObject( format( "World_MainChar{}", i ).c_str() ); go_MainChar2->Enable();
		auto c_Chars_Sp2 = new Spirte( format( "Sprite{}", i ).c_str(), "Chars" );
		go_MainChar2->AddComponent( c_Chars_Sp2 );
		auto c_MainChar_Tr2 = go_MainChar2->GetComponent<Transform>();
		c_MainChar_Tr2->SetGlobalRect( 20.f * i, 10.f * i, 50, 50 );
		auto c_Chars_Rb2 = static_cast< RigidBodyB2* >( go_MainChar2->AddComponent( new RigidBodyB2( HG_ENGINE()->tPhyiscs.FirstWorld(), c_MainChar_Tr2, true, format( "World_MainChar2_RigidBody{}", i ).c_str() ) ) );
	}
}

HG_SCRIPT_START( SCRIPT_WORLD ) {

	auto* s = HGX::GetSceneWithGUI( "World" );
	// auto* canvas = s->GetGUI( "Canvas" );
	EngineImpl::GetEngine()->NavigateScene( s );
	
	// GameObject* go_World = new GameObject("World"); go_World->Enable();

	
	auto* pCamera = s->GetMainCamera();
	auto df3 = s->GetMainCamera()->GetComponent<Transform>();
	df3->tPosition.X = 0;
	df3->tPosition.Y = 0;

	//canvas->OnGUI = HG_EVENT_IMPL {
	//	auto gui = static_cast< GUI* >( pThis );
	//	return 0;
	//};

	// Ground
	GameObject* go_Ground = new GameObject( "World_Ground" ); go_Ground->Enable();
	auto c_Bg_Sp = new Spirte( "Sprite", "Bg" );
	go_Ground->AddComponent( c_Bg_Sp );
	auto c_Bg_Tr = go_Ground->GetComponent<Transform>();
	c_Bg_Tr->tPosition.X = -200;
	c_Bg_Tr->tPosition.Y = -200;
	c_Bg_Tr->tRect.W = 1200;
	c_Bg_Tr->tRect.H = 1200;

	// Main Character
	GameObject* go_MainChar = new GameObject( "World_MainChar" ); go_MainChar->Enable();
	auto c_Chars_Sp = new Spirte( "Sprite3", "Chars" );
	go_MainChar->AddComponent( c_Chars_Sp );
	auto c_MainChar_Tr = go_MainChar->GetComponent<Transform>();
	c_MainChar_Tr->SetGlobalRect( 300, 400, 50, 50 );
	auto c_Chars_Rb = static_cast< RigidBodyB2* >( go_MainChar->AddComponent( new RigidBodyB2( HG_ENGINE()->tPhyiscs.FirstWorld(), c_MainChar_Tr, true, "World_MainChar_RigidBody" ) ) );

	GameObject* go_MainChar3 = new GameObject( "World_MainChar3" ); go_MainChar3->Enable();
	auto c_Chars_Sp3 = new Spirte( "Sprite4", "Chars" );
	go_MainChar3->AddComponent( c_Chars_Sp3 );
	auto c_MainChar_Tr3 = go_MainChar3->GetComponent<Transform>();
	c_MainChar_Tr3->SetGlobalRect( 0, 700, 1000, 50 );
	auto c_Chars_Rb3 = static_cast< RigidBodyB2* >( go_MainChar3->AddComponent( new RigidBodyB2( HG_ENGINE()->tPhyiscs.FirstWorld(), c_MainChar_Tr3, false, "World_MainChar_RigidBody3" ) ) );

	GameObject* go_MainChar2 = new GameObject( "World_MainChar2" ); go_MainChar2->Enable();
	auto c_Chars_Sp2 = new Spirte( "Sprite2", "Chars" );
	go_MainChar2->AddComponent( c_Chars_Sp2 );
	auto c_MainChar_Tr2 = go_MainChar2->GetComponent<Transform>();
	c_MainChar_Tr2->SetGlobalRect( 0,0,50,50 );
	auto c_Chars_Rb2 = static_cast< RigidBodyB2* >( go_MainChar2->AddComponent( new RigidBodyB2( HG_ENGINE()->tPhyiscs.FirstWorld(), c_MainChar_Tr2, true, "World_MainChar2_RigidBody2" ) ) );
	CreateC();
	
	HG_EVENT_BIND( s->GetMainCamera(), OnFixedUpdate ) {
		auto _this = HG_EVENT_THIS_GAMEOBJECT;
		auto c_Camera_Tr = _this->GetComponent<Transform>();
		auto eff = _this->GetComponent<Effect>();
		auto go_MainChar = GameObject::Find( "World_MainChar" );
		auto c_MainChar_Tr = go_MainChar->GetComponent<Transform>();
	
		auto go_Bg = GameObject::Find( "World_Ground" );
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
		// eff->Play( HG_ENGINE_TIMEDELTA, Effect::Fading::HG_EFFECT_FADING_OUT );
		return 0;
	};

	HG_EVENT_BIND( go_MainChar, OnFixedUpdate ) {
		auto _this = HG_EVENT_THIS_GAMEOBJECT;
		auto rb = _this->GetComponent<RigidBodyB2>();
		//auto an = _this->GetComponent<Animator2D>();
		//an->IsIdle = false;
		//HG::Math::HGVec2<float> a;
		//a.X = 200;
		switch( HG_ENGINE_INPUT()->GetDirect() ) {
		case HG::HGInput::UP:
		rb->ApplyForce( b2Vec2( 0, -100.f ) );
		//rb->MovePosition( rb->Velocity.X, -100 );
		//an->Row = 3;
		//an->ForceSetFrame();
		break;
		case HG::HGInput::DOWN:
		//rb->MovePosition( rb->Velocity.X, 100 );
		//an->Row = 0;
		//an->ForceSetFrame();
		break;
		case HG::HGInput::LEFT:
		rb->ApplyForce( b2Vec2( -50.0f, 0.f ) );
		//rb->MovePosition( -100, rb->Velocity.Y );
		//an->Row = 1;
		//an->ForceSetFrame();
		break;
		case HG::HGInput::RIGHT:
		rb->ApplyForce( b2Vec2( 50.0f, 0.f ) );
		//rb->MovePosition( 100, rb->Velocity.Y );
		//an->Row = 2;
		//an->ForceSetFrame();
		break;
		case HG::HGInput::UPL:
		//rb->MovePosition( -100, rb->Velocity.Y );
		//rb->MovePosition( rb->Velocity.X, -100 );
		//an->Row = 3;
		//an->ForceSetFrame();
		break;
		case HG::HGInput::UPR:
		//rb->MovePosition( 100, rb->Velocity.Y );
		//rb->MovePosition( rb->Velocity.X, -100 );
		//an->Row = 3;
		//an->ForceSetFrame();
		break;
		case HG::HGInput::DOWNL:
		//rb->MovePosition( -100, rb->Velocity.Y );
		//rb->MovePosition( rb->Velocity.X, 100 );

		//an->Row = 0;
		//an->ForceSetFrame();
		break;
		case HG::HGInput::DOWNR:
		//rb->MovePosition( 100, rb->Velocity.Y );
		//rb->MovePosition( rb->Velocity.X, 100 );
		//an->Row = 0;
		//an->ForceSetFrame();
		break;
		default:
		break;
		}
		//if( HG::Math::Abs( rb->Velocity.X ) <= 5 ) {
		//	an->IsIdle = true;
		//}
		//if( an != nullptr ) {
		//	an->Play( HG_ENGINE_TIMEDELTA );
		//}
		return 0;
	};

} HG_SCRIPT_END