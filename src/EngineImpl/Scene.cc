//
// Created by cyf-m on 2020/11/28.
//

#include "Scene.h"
#include "GameObject.h"
#include "../Core/Log.h"

using namespace __HGImpl::V1;
using namespace HGCore;

Scene::Scene(const char *strName): Object<Scene>(strName) {
    HG_LOG_INFO( std::string("scene [").append(GetName()).append("] constructed").c_str() );
}

Scene::~Scene() {
    HG_LOG_INFO( std::string("scene [").append(GetName()).append("] destructed").c_str() );
    auto it = umGameObjectsByName.begin();
    while ( it != umGameObjectsByName.end() ) {
        HG_SAFE_DEL( it->second );
    }
}

void Scene::AttachGameObject( GameObject *pGameObject ) {
    if ( umGameObjectsByName.count( pGameObject->GetName() ) == 0 ) {
        umGameObjectsByName[pGameObject->GetName()] = pGameObject;
        // pGameObject->OnAttach();
        HG_LOG_INFO( std::string("attached [").append(pGameObject->GetName()).append("] to scene: ").append(GetName()).c_str());
    } else {
        Log->Warning( SDL_LOG_CATEGORY_SYSTEM,std::string("trying to attach a same name game object, which will do not recover the original one. Name: ").append(pGameObject->GetName()).c_str());
    }
}

GameObject *Scene::FindGameObject(const char *strName) {
    return umGameObjectsByName.count( strName ) == 0 ? nullptr : umGameObjectsByName[strName];
}

void Scene::renderAllGameObjects( HGCore::Renderer* pRd ) {
    for(auto& it : umGameObjectsByName ) {
        if ( it.second->IsEnable() ) {
            it.second->Render( pRd );
        }
    }
}

void Scene::updateAllGameObjects( void *pEv ) {
    for(auto& it : umGameObjectsByName ) {
        if ( it.second->IsEnable() ) {
            it.second->Update( pEv );
        }
    }
}
