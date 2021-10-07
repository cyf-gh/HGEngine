#include "Transform.hpp"
#include "GameObject.h"
#include "RigidBody.h"

void __HGImpl::V1SDL::RigidBody::Proc( f32 deltaTime ) {
	if( IsFrozen ) {
		return;
	}
	m_pGameObject->GetComponent<Transform>()->tPosition.X += Velocity.X * deltaTime;
	m_pGameObject->GetComponent<Transform>()->tPosition.Y += Velocity.Y * deltaTime;

	auto nextVX = Velocity.X > 0 ? Velocity.X - LinearDrag * deltaTime : Velocity.X + LinearDrag * deltaTime;
	// auto nextVY = Velocity.Y > 0 ? Velocity.Y - LinearDrag * deltaTime : Velocity.Y + LinearDrag * deltaTime;

	if( Velocity.X > 0 ) {
		Velocity.X = nextVX <= 0 ? 0 : nextVX;
	} else {
		Velocity.X = nextVX >= 0 ? 0 : nextVX;
	}
	/*
	if( Velocity.Y > 0 ) {
	 	Velocity.Y = nextVY <= 0 ? 0 : nextVY;
	} 
	else {
		Velocity.Y = nextVY >= 0 ? 0 : nextVY;
	}
	*/
	Velocity.Y += GravityDrag * deltaTime;
}

void __HGImpl::V1SDL::RigidBody::MovePosition( HG::Math::HGVec2<f32> vec2d ) {
	MovePosition( vec2d.X, vec2d.Y );
}

void __HGImpl::V1SDL::RigidBody::MovePosition( const f32 x, const f32 y ) {
	Velocity.X = x;
	Velocity.Y = y;
}
