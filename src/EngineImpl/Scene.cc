//
// Created by cyf-m on 2020/11/28.
//

#include "Scene.h"
#include "GameObject.h"
#include "../Core/Log.h"
#include "EngineImpl.h"

using namespace __HGImpl::V1SDL;
using namespace __HGImpl;

using namespace HG::V1SDL;

Scene::Scene( const char* strName ) : HGObject<Scene>( strName ) {
	HG_LOG_INFO( std::string( "scene [" ).append( GetName() ).append( "] constructed" ).c_str() );
}

Scene::~Scene() {
	for( auto& it : umGameObjectsByName ) {
		HG_SAFE_DEL( it.second );
	}
	HG_LOG_INFO( std::string( "scene [" ).append( GetName() ).append( "] destructed" ).c_str() );
}

void Scene::AttachGameObject( GameObject* pGameObject ) {
	if( umGameObjectsByName.count( pGameObject->GetName() ) == 0 ) {
		umGameObjectsByName[pGameObject->GetName()] = pGameObject;
		// pGameObject->OnAttach();
		HG_LOG_INFO( std::string( "attached [" ).append( pGameObject->GetName() ).append( "] to scene: " ).append( GetName() ).c_str() );
	} else {
		Log->Warning( SDL_LOG_CATEGORY_SYSTEM, std::string( "trying to attach a same name game object, which will do not recover the original one. Name: " ).append( pGameObject->GetName() ).c_str() );
	}
}

GameObject* Scene::FindGameObject( const char* strName ) {
	return umGameObjectsByName.count( strName ) == 0 ? nullptr : umGameObjectsByName[strName];
}

void __HGImpl::V1SDL::Scene::Update( void* pEvent ) {
	HG_EVENT_CALL( OnFixedUpdate, pEvent, this );
	updateAllGameObjects( pEvent );
}

void __HGImpl::V1SDL::Scene::Render( void* pRenderer ) {
	HG_EVENT_CALL( OnUpdate, pRenderer, this );
	HG_EVENT_CALL( OnRender, pRenderer, this );
	renderAllGameObjects( static_cast< Renderer* >( pRenderer ) );
}

void Scene::renderAllGameObjects( __HGImpl::Renderer* pRd ) {
	for( auto& it : umGameObjectsByName ) {
		if( it.second->IsEnable() ) {
			if( HG_EVENT_CALLABLE( it.second->OnUpdate ) ) {
				HG_EVENT_CALL( it.second->OnUpdate, &HGMainLoop::tEvent, this );
			}
			it.second->Render( pRd );
		}
	}
}

void Scene::updateAllGameObjects( void* pEv ) {
	for( auto& it : umGameObjectsByName ) {
		if( it.second->IsEnable() ) {
			it.second->Update( pEv );
		}
	}
}
