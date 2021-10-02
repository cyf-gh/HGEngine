//
// Created by cyf-m on 2020/12/8.
//

#include "../Core/Log.h"
#include "GameObject.h"
#include "EngineImpl.h"
#include "Scene.h"

using namespace __HGImpl::V1SDL;
using namespace __HGImpl;

GameObject::GameObject( const char* strName, Scene* pScene ) 
: HGObject<GameObject>( strName ), m_pScene( pScene ), m_vecComponents() {
    HG_EVENT_CALL( OnBeforeConstruct, nullptr, this );
    if ( m_pScene == nullptr ) {
        m_pScene = EngineImpl::GetEngine()->GetCurrentScene();
    }
    if ( m_pScene != nullptr ) {
        HG_LOG_INFO( std::string("game object [").append(GetName()).append("] constructed").c_str() );
        m_pScene->AttachGameObject( this );
    }
}

GameObject::~GameObject() {
    for( auto& c : m_vecComponents ) {
        HG_LOG_INFO( (std::string("game object [").append(GetName()) + "] component [" + c->GetName() + "] destructed").c_str() );
        HG_SAFE_DEL( c );
    }
    HG_LOG_INFO( std::string("game object [").append(GetName()).append("] destructed").c_str() );
}
