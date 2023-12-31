#include <box2d\box2d.h>
#include "Transform.hpp"
#include "GameObject.h"
#include "RigidBody.h"

using namespace HGEngine::V1SDL;
#pragma region HG
void HGEngine::V1SDL::RigidBody::Proc( f32 deltaTime ) {
	if( IsFrozen ) {
		return;
	}
	m_pGameObject->GetComponent<Transform>()->tPosition.X += Velocity.X * deltaTime;
	m_pGameObject->GetComponent<Transform>()->tPosition.Y += Velocity.Y * deltaTime;

	auto nextVX = Velocity.X > 0 ? Velocity.X - LinearDrag * deltaTime : Velocity.X + LinearDrag * deltaTime;

	if( Velocity.X > 0 ) {
		Velocity.X = nextVX <= 0 ? 0 : nextVX;
	} else {
		Velocity.X = nextVX >= 0 ? 0 : nextVX;
	}
	if( Mode == Modes::Plain2D ) {
		auto nextVY = Velocity.Y > 0 ? Velocity.Y - LinearDrag * deltaTime : Velocity.Y + LinearDrag * deltaTime;
		if( Velocity.Y > 0 ) {
	 		Velocity.Y = nextVY <= 0 ? 0 : nextVY;
		} 
		else {
			Velocity.Y = nextVY >= 0 ? 0 : nextVY;
		}
	} else {
		Velocity.Y += GravityDrag * deltaTime;
	}
}

void HGEngine::V1SDL::RigidBody::MovePosition( HG::Math::HGVec2<f32> vec2d ) {
	MovePosition( vec2d.X, vec2d.Y );
}

void HGEngine::V1SDL::RigidBody::MovePosition( const f32 x, const f32 y ) {
	Velocity.X = x;
	Velocity.Y = y;
}
#pragma endregion
#pragma region box2d

void HGEngine::V1SDL::RigidBodyB2::Sync2Transform() {
	auto tr = m_pGameObject->GetComponent<Transform>();
	auto posVec = pBody->GetPosition();
	auto angle = pBody->GetAngle();
	tr->f64Angle = angle;
	tr->tPosition.X = ( posVec.x * PPM ) - ( tr->tRect.W / 2 );
	tr->tPosition.Y = ( posVec.y * PPM ) - ( tr->tRect.H / 2 );
}

void HGEngine::V1SDL::RigidBodyB2::ApplyForce( const b2Vec2& vec ) {
	pBody->ApplyForceToCenter( vec, true );
}

HGEngine::V1SDL::RigidBodyB2::RigidBodyB2( HGWorld* pworld, Transform *tr, bool isDynmaic, const char* strName ) : pWorld( pworld), IsSyncTransform( true ), HGComponent( strName ) {
	pWorld->vecRbs.push_back( this );
	auto rect = tr->ToHGRectGlobal();
	tBodyDef.type = isDynmaic ? b2_dynamicBody : b2_staticBody;
	// body def
	tBodyDef.position.Set( rect.GetCenter().X / PPM, rect.GetCenter().Y / PPM );
	// body
	
	pBody = pWorld->Handle.CreateBody( &tBodyDef );
	// shape
	tBox.SetAsBox( tr->tRect.W / PPM / 2.0f, tr->tRect.H / PPM / 2.0f );
	
	tFixtureDef.shape = &tBox;
	tFixtureDef.density = 1.0f;
	tFixtureDef.friction = 0.3f;
	
	// Add the ground fixture to the ground body.
	pFixture = pBody->CreateFixture( &tFixtureDef );
	pBody->SetAwake( true );
}

HGEngine::V1SDL::RigidBodyB2::~RigidBodyB2() { 
	pBody->DestroyFixture( pFixture );
	pWorld->Handle.DestroyBody( pBody );
	HG_ERASE_IN_VEC( this, pWorld->vecRbs );
}

#pragma endregion
