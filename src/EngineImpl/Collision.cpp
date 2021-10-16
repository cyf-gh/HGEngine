#include <typeinfo>
#include <algorithm>
#include <list>
#include "Collision.h"
#include "RigidBody.h"
#include "Transform.hpp"

char HGEngine::V1SDL::BoxCollision::checkWhichSideCol( GameObject* pRectColObj ) {
	auto tr2 = pRectColObj->GetComponent<BoxCollision>();

	if( Rect.Bottom() >= tr2->Rect.Top() ) { return HG_BC_BOTTOM; }
	if( Rect.Top() <= tr2->Rect.Bottom() ) { return HG_BC_TOP; }
	if( Rect.Right() >= tr2->Rect.Left() ) { return HG_BC_RIGHT; }
	if( Rect.Left() <= tr2->Rect.Right() ) { return HG_BC_LEFT; }
	return HG_BC_NONE;
}

bool HGEngine::V1SDL::BoxCollision::CanLeave( GameObject* pRectColObj ) {
	auto rb1 = pRectColObj->GetComponent<RigidBody>();
	auto rbThis = m_pGameObject->GetComponent<RigidBody>();
	if( rb1 == nullptr || rbThis == nullptr ) {
		return false;
	}
	if( rbThis->IsFrozen ) {
		return false;
	}
	auto key = checkWhichSideCol( pRectColObj );
	switch( key ) {
	case HG_BC_TOP:
	if( rbThis->Velocity.Y < 0 ) { rbThis->Velocity.Y = 0; return true; } break;
	case HG_BC_BOTTOM:
	if( rbThis->Velocity.Y > 0 ) { rbThis->Velocity.Y = 0; return true; } break;
	case HG_BC_LEFT:
	if( rbThis->Velocity.X < 0 ) { rbThis->Velocity.X = 0; return true; } break;
	case HG_BC_RIGHT:
	if( rbThis->Velocity.X > 0 ) { rbThis->Velocity.X = 0; return true; } break;
	}
	return false;
}

bool HGEngine::V1SDL::BoxCollision::DoCheck( GameObject* pObj ) {
	auto pC = pObj->GetComponent<Collision>( "Collision" );
	this->SetCollisionBoundingByTransform();
	if( pC == nullptr ) {
		return false;
	}
	pC->SetCollisionBoundingByTransform();
	bool collided = false;
	if( pC == nullptr ) {
		return false;
	}
	if( typeid( *pC ) == typeid( BoxCollision ) ) {
		collided = Rect.IsOverlap( static_cast< BoxCollision* >( pC )->Rect );
	}
	if( typeid( *pC ) == typeid( CircleCollision ) ) {
		collided = Rect.IsOverlap( static_cast< CircleCollision* >( pC )->Circle );
	}
	procCollided( collided, pObj );
	return collided;
}



void HGEngine::V1SDL::BoxCollision::SetCollisionBoundingByTransform() {
	auto pC = m_pGameObject->GetComponent<Transform>();
	this->Rect = HG::Math::HGRect {
		.X = ( int ) pC->tPosition.X,
		.Y = ( int ) pC->tPosition.Y,
		.H = ( un32 ) pC->tRect.H,
		.W = ( un32 ) pC->tRect.W,
	};
}

bool HGEngine::V1SDL::CircleCollision::DoCheck( GameObject* pObj ) {
	auto pC = pObj->GetComponent<Collision>();
	bool collided = false;
	if( pC == nullptr ) {
		return false;
	}
	if( typeid( *pC ) == typeid( BoxCollision ) ) {
		collided = static_cast< BoxCollision* >( pC )->Rect.IsOverlap( Circle );
	}
	if( typeid( *pC ) == typeid( CircleCollision ) ) {
		collided = Circle.IsOverlap( static_cast< CircleCollision* >( pC )->Circle );
	}
	procCollided( collided, pObj );
	return collided;
}

void HGEngine::V1SDL::Collision::procCollided( bool collided, GameObject* pObj ) {
	if( std::find( m_lColList.begin(), m_lColList.end(), pObj ) != m_lColList.end() ) {
		if( collided ) {
			// 之前已经碰撞过且继续碰撞
			HG_EVENT_CALL( OnCollisionStay, pObj, this->m_pGameObject );
			HG_EVENT_CALL( OnCollisionStay, this->m_pGameObject, pObj );
		} else {
			// 之前已经碰撞过但现在不碰撞
			HG_EVENT_CALL( OnCollisionExit, pObj, this->m_pGameObject );
			HG_EVENT_CALL( OnCollisionExit, this->m_pGameObject, pObj );
			for( auto itList = m_lColList.begin(); itList != m_lColList.end(); ) {
				if( *itList == pObj ) {
					m_lColList.erase( itList++ );
					break;
				} else {
					itList++;
				}
			}
		}
	} else {
		if( collided ) {
			HG_EVENT_CALL( OnCollisionEnter, pObj, this->m_pGameObject );
			HG_EVENT_CALL( OnCollisionEnter, this->m_pGameObject, pObj );
			auto rb1 = pObj->GetComponent<Collision>("Collision");
			auto rbThis = m_pGameObject->GetComponent<Collision>("Collision");
			auto a = rb1->CanLeave( pObj );
			auto b = rbThis ->CanLeave( m_pGameObject );
			if ( a || b )
			{
				return;
			}
			m_lColList.push_back( pObj );
		}
	}
}
