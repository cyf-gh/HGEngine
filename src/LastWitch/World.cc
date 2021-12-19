#include <engine/HGEngine.hpp>

HG_SCRIPT_START( SCRIPT_WORLD )

auto* s = HGX::GetSceneWithGUI( "World" );
auto* canvas = s->GetGUI( "Canvas" );

auto df3 = s->GetMainCamera()->GetComponent<Transform>();
df3->tPosition.X = 0;
df3->tPosition.Y = 0;

canvas->OnGUI = HG_EVENT_IMPL {
	auto gui = static_cast< GUI* >( pThis );
	return 0;
};

GameObject* go_MainChar = new GameObject( "World_MainChar" ); go_MainChar->Enable();
auto c_Chars_Sp = new Spirte( "Sprite", "Chars" );
go_MainChar->AddComponent( c_Chars_Sp );

auto dfMain = go_MainChar->GetComponent<Transform>();
dfMain->tPosition.X = 200;
dfMain->tPosition.Y = 400;
dfMain->tRect.W = 50;
dfMain->tRect.H = 50;

auto c_Chars_An = static_cast< Animator2D* >( go_MainChar->AddComponent( new Animator2D( "World_MainChar_Animator", HGSize<un32>( 48, 48 ), 3, 4, 1, 0.4f, true ) ) );
auto c_Chars_BoxCol = static_cast< BoxCollision* >( go_MainChar->AddComponent( new BoxCollision( "World_MainChar_Collision" ) ) );
auto c_Chars_Rb = static_cast< RigidBody* >( go_MainChar->AddComponent( new RigidBody( "World_MainChar_RigidBody" ) ) );
c_Chars_Rb->LinearDrag = 50.f;

HG_EVENT_BIND( s->GetMainCamera(), OnFixedUpdate ) {
	auto _this = HG_EVENT_THIS_GAMEOBJECT;
	auto df = _this->GetComponent<Transform>();
	auto eff = _this->GetComponent<Effect>();
	eff->Play( HG_ENGINE_TIMEDELTA, Effect::Fading::HG_EFFECT_FADING_OUT );
	auto res = HG_ENGINE_INPUT()->GetDirect();
	if( res != HG::HGInput::Direct::STAY ) {
		HG_LOG_INFO( std::format( "{}", ( int ) res ).c_str() );
	}
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

HG_SCRIPT_END