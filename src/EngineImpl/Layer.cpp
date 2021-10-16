#include <algorithm>
#include "../Core/Log.h"
#include "Scene.h"
#include "Transform.hpp"
#include "Layer.h"
#include "Collision.h"
#include "RigidBody.h"
#include "EngineImpl.h"

using namespace HGEngine::V1SDL;
using namespace std;

void HGEngine::V1SDL::Layer::DoCheck() {
	int j = 0;
	for( auto& x : m_vecX ) {
		Collision* col = x->GetComponent<BoxCollision>( "Collision" );
		if( col == nullptr ) {
			goto CONTI;
		}
		for( size_t i = j + 1; i < m_vecX.size(); ++i ) {
			col->DoCheck( m_vecX[i] );
		}
CONTI:
		j++;
	}
	for( auto& x : m_vecX ) {
		RigidBody* rb = x->GetComponent<RigidBody>();
		if( rb != nullptr ) {
			rb->Proc( HG_ENGINE_TIMEDELTA );
		}
	}
}

void HGEngine::V1SDL::Layer::AttachGameObject( GameObject* pGameObject ) {
	HG_EVENT_CALL( OnAttachToLayer, &LayerIndex, pGameObject );
	pGameObject->SetLayer( this );
	m_vecObjs.push_back( pGameObject );
	HG_LOG_INFO( std::format( "Scene[{}]->Layer[{}]->GameObject[{}] ! Attached", pGameObject->GetScene()->GetName(), std::to_string( LayerIndex ), pGameObject->GetName() ).c_str() );
	SortXY( pGameObject );
}

bool HGEngine::V1SDL::Layer::DetachGameObject( GameObject* pGameObject ) {
	return DetachGameObject( pGameObject->UID );
}

bool HGEngine::V1SDL::Layer::DetachGameObject( const char* strName ) {
	return DetachGameObject( GameObject::Find( strName ) );
}

bool HGEngine::V1SDL::Layer::DetachGameObject( const un32 Id ) {
	vector<int>::iterator it;
	bool detached = false;
	for( int i = 0; i < m_vecObjs.size(); ++i ) {
		if( m_vecObjs[i]->UID == Id ) {
			HG_EVENT_CALL( OnDetachFromLayer, &LayerIndex, m_vecObjs[i] );
			m_vecObjs[i]->SetLayer( nullptr );
			m_vecObjs.erase( m_vecObjs.begin() + i );
			detached = true;
		}
	}
	for( int i = 0; i < m_vecX.size(); ++i ) {
		if( m_vecX[i]->UID == Id ) {
			m_vecX.erase( m_vecX.begin() + i );
			break;
		}
	}
	for( int i = 0; i < m_vecY.size(); ++i ) {
		if( m_vecY[i]->UID == Id ) {
			m_vecY.erase( m_vecY.begin() + i );
			break;
		}
	}
	return false;
}

un32 HGEngine::V1SDL::Layer::SortXY( GameObject* pGameObject ) {
	m_vecX.push_back( pGameObject );
	std::sort( m_vecX.begin(), m_vecX.end(), []( GameObject* pA, GameObject* pB )-> bool {
		auto ta = pA->GetComponent<Transform>();
		auto tb = pB->GetComponent<Transform>();
		return ta->tPosition.X < tb->tPosition.X;
	} );
	m_vecY.push_back( pGameObject );
	std::sort( m_vecY.begin(), m_vecY.end(), []( GameObject* pA, GameObject* pB )-> bool {
		auto ta = pA->GetComponent<Transform>();
		auto tb = pB->GetComponent<Transform>();
		return ta->tPosition.Y < tb->tPosition.Y;
	} );
	return 0;
}

