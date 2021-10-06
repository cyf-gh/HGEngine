#include <typeinfo>
#include <algorithm>
#include <list>
#include "Collision.h"
#include "Transform.hpp"

bool __HGImpl::V1SDL::BoundingCollision::DoCheck( GameObject* pObj ) {
	auto pC = pObj->GetComponent<Collision>();
	bool collided = false;
	if( pC == nullptr ) {
		return false;
	}
	if( typeid( *pC ) == typeid( BoundingCollision ) ) {
		collided = Rect.IsIntersect( static_cast< BoundingCollision* >( pC )->Rect );
	}
	if( typeid( *pC ) == typeid( CircleCollision ) ) {
		collided = Rect.IsIntersect( static_cast< CircleCollision* >( pC )->Circle );
	}
	procCollided( collided, pObj );
	return collided;
}

bool __HGImpl::V1SDL::CircleCollision::DoCheck( GameObject* pObj ) {
	auto pC = pObj->GetComponent<Collision>();
	bool collided = false;
	if( pC == nullptr ) {
		return false;
	}
	if( typeid( *pC ) == typeid( BoundingCollision ) ) {
		collided = static_cast< BoundingCollision* >( pC )->Rect.IsIntersect( Circle );
	}
	if( typeid( *pC ) == typeid( CircleCollision ) ) {
		collided = Circle.IsIntersect( static_cast< CircleCollision* >( pC )->Circle );
	}
	procCollided( collided, pObj );
	return collided;
}

void __HGImpl::V1SDL::Collision::procCollided( bool collided, GameObject* pObj  ) {
	if( std::find( m_lColList.begin(), m_lColList.end(), pObj ) != m_lColList.end() ) {
		if( collided ) {
			// 之前已经碰撞过且继续碰撞
			HG_EVENT_CALL( OnCollisionStay, pObj, this->m_pGameObject );
		} else {
			// 之前已经碰撞过但现在不碰撞
			HG_EVENT_CALL( OnCollisionExit, pObj, this->m_pGameObject );
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
			m_lColList.push_back( pObj );
		}
	}
}
