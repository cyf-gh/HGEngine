#include "Layer.h"

using namespace __HGImpl::V1SDL;
using namespace std;

void __HGImpl::V1SDL::Layer::AttachGameObject( GameObject* pGameObject ) { 
	HG_EVENT_CALL( OnAttachToLayer, &LayerIndex, pGameObject ); 
	pGameObject->SetLayer( this );
	m_vecObjs.push_back( pGameObject );
}

bool __HGImpl::V1SDL::Layer::DetachGameObject( GameObject* pGameObject ) {
	return DetachGameObject( pGameObject->UID );
}

bool __HGImpl::V1SDL::Layer::DetachGameObject( const char* strName ) {
	vector<int>::iterator it;
	for( int i = 0; i < m_vecObjs.size(); ++i ) {
		if( 0 == strcmp( m_vecObjs[i]->GetName(), strName ) ) {
			HG_EVENT_CALL( OnDetachFromLayer, &LayerIndex, m_vecObjs[i] );
			m_vecObjs[i]->SetLayer( nullptr );
			m_vecObjs.erase( m_vecObjs.begin() + i );
			return true;
		}
	}
	return false;
}

bool __HGImpl::V1SDL::Layer::DetachGameObject( const un32 Id ) {
	vector<int>::iterator it;
	for( int i = 0; i < m_vecObjs.size(); ++i ) {
		if( m_vecObjs[i]->UID == Id ) {
			HG_EVENT_CALL( OnDetachFromLayer, &LayerIndex, m_vecObjs[i] );
			m_vecObjs[i]->SetLayer( nullptr );
			m_vecObjs.erase( m_vecObjs.begin() + i );
			return true;
		}
	}
	return false;
}

