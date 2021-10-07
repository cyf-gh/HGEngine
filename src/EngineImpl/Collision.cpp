#include <typeinfo>
#include <algorithm>
#include <list>
#include "Collision.h"
#include "Transform.hpp"

bool __HGImpl::V1SDL::BoxCollision::DoCheck( GameObject* pObj ) {
	auto pC = pObj->GetComponent<Collision>("Collision");
	this->SetCollisionBoundingByTransform();
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

void __HGImpl::V1SDL::BoxCollision::SetCollisionBoundingByTransform() {
	auto pC = m_pGameObject->GetComponent<Transform>();
	this->Rect = HG::Math::HGRect {
		.X = (int)pC->tPosition.X,
		.Y = (int)pC->tPosition.Y,
		.H = (un32)pC->tRect.H,
		.W = (un32)pC->tRect.W,
	};
}

bool __HGImpl::V1SDL::CircleCollision::DoCheck( GameObject* pObj ) {
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

void __HGImpl::V1SDL::Collision::procCollided( bool collided, GameObject* pObj  ) {
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
			m_lColList.push_back( pObj );
		}
	}
}
