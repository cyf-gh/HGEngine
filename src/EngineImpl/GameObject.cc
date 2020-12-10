//
// Created by cyf-m on 2020/12/8.
//

#include "../Core/Log.h"
#include "GameObject.h"
#include "EngineImpl.h"
#include "Scene.h"

using namespace __HGImpl::V1;
using namespace HGCore;

GameObject::GameObject( const char* strName, Scene* pScene ) : Object<GameObject>( strName ), m_pScene( pScene ) {
    if ( m_pScene == nullptr ) {
        m_pScene = EngineImpl::GetEngine()->GetCurrentScene();
    }
    HG_LOG_INFO( std::string("game object [").append(GetName()).append("] constructed").c_str() );
    m_pScene->AttachGameObject( this );
}

GameObject::~GameObject() {
    HG_LOG_INFO( std::string("game object [").append(GetName()).append("] destructed").c_str() );
}
