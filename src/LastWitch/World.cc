#include <engine/HGEngine.hpp>

HG_SCRIPT_START( SCRIPT_WORLD ) {

	auto* s = HGX::GetSceneWithGUI( "World" );
	auto* canvas = s->GetGUI( "Canvas" );
	EngineImpl::GetEngine()->NavigateScene( "World" );

	auto* pCamera = s->GetMainCamera();
	auto df3 = s->GetMainCamera()->GetComponent<Transform>();
	df3->tPosition.X = 0;
	df3->tPosition.Y = 0;

	canvas->OnGUI = HG_EVENT_IMPL {
		auto gui = static_cast< GUI* >( pThis );
		return 0;
	};

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
	auto c_Chars_Sp = new Spirte( "Sprite", "Chars" );
	go_MainChar->AddComponent( c_Chars_Sp );
	auto c_MainChar_Tr = go_MainChar->GetComponent<Transform>();
	c_MainChar_Tr->tPosition.X = 200;
	c_MainChar_Tr->tPosition.Y = 400;
	c_MainChar_Tr->tRect.W = 50;
	c_MainChar_Tr->tRect.H = 50;
	auto c_Chars_An = static_cast< Animator2D* >( go_MainChar->AddComponent( new Animator2D( "World_MainChar_Animator", HGSize<un32>( 48, 48 ), 3, 4, 1, 0.4f, true ) ) );
	auto c_Chars_BoxCol = static_cast< BoxCollision* >( go_MainChar->AddComponent( new BoxCollision( "World_MainChar_Collision" ) ) );
	auto c_Chars_Rb = static_cast< RigidBody* >( go_MainChar->AddComponent( new RigidBody( "World_MainChar_RigidBody" ) ) );
	c_Chars_Rb->LinearDrag = 50.f;

	HG_EVENT_BIND( GameObject::Find( "World_MainCamera" ), OnFixedUpdate ) {
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
		eff->Play( HG_ENGINE_TIMEDELTA, Effect::Fading::HG_EFFECT_FADING_OUT );
		return 0;
	};

	HG_EVENT_BIND( go_MainChar, OnFixedUpdate ) {
		auto _this = HG_EVENT_THIS_GAMEOBJECT;
		auto rb = _this->GetComponent<RigidBody>();
		auto an = _this->GetComponent<Animator2D>();
		an->IsIdle = false;
		HG::Math::HGVec2<float> a;
		a.X = 200;
		switch( HG_ENGINE_INPUT()->GetDirect() ) {
		case HG::HGInput::UP:
		rb->MovePosition( rb->Velocity.X, -100 );
		an->Row = 3;
		an->ForceSetFrame();
		break;
		case HG::HGInput::DOWN:
		rb->MovePosition( rb->Velocity.X, 100 );
		an->Row = 0;
		an->ForceSetFrame();
		break;
		case HG::HGInput::LEFT:
		rb->MovePosition( -100, rb->Velocity.Y );
		an->Row = 1;
		an->ForceSetFrame();
		break;
		case HG::HGInput::RIGHT:
		rb->MovePosition( 100, rb->Velocity.Y );
		an->Row = 2;
		an->ForceSetFrame();
		break;
		case HG::HGInput::UPL:
		rb->MovePosition( -100, rb->Velocity.Y );
		rb->MovePosition( rb->Velocity.X, -100 );
		an->Row = 3;
		an->ForceSetFrame();
		break;
		case HG::HGInput::UPR:
		rb->MovePosition( 100, rb->Velocity.Y );
		rb->MovePosition( rb->Velocity.X, -100 );
		an->Row = 3;
		an->ForceSetFrame();
		break;
		case HG::HGInput::DOWNL:
		rb->MovePosition( -100, rb->Velocity.Y );
		rb->MovePosition( rb->Velocity.X, 100 );

		an->Row = 0;
		an->ForceSetFrame();
		break;
		case HG::HGInput::DOWNR:
		rb->MovePosition( 100, rb->Velocity.Y );
		rb->MovePosition( rb->Velocity.X, 100 );
		an->Row = 0;
		an->ForceSetFrame();
		break;
		default:
		break;
		}
		if( HG::Math::Abs( rb->Velocity.X ) <= 5 ) {
			an->IsIdle = true;
		}
		if( an != nullptr ) {
			an->Play( HG_ENGINE_TIMEDELTA );
		}
		return 0;
	};

} HG_SCRIPT_END