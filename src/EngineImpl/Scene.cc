//
// Created by cyf-m on 2020/11/28.
//

#include "Scene.h"
#include "GameObject.h"
#include "../Core/Log.h"
#include "EngineImpl.h"

using namespace __HGImpl::V1SDL;
using namespace __HGImpl;
using namespace HG;
using namespace std;

Scene::Scene( const char* strName )
	: HGObject<Scene>( strName ), m_pMainCamera( nullptr ), m_vecLayers(), OnAttach( nullptr ) {
	for( int i = 0; i < HG_LAYER_LENGTH; ++i ) {
		m_vecLayers.push_back( new Layer( ( string( "Layer" ) + to_string( i ) ).c_str(), i ) );
	}
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
		HG_EVENT_CALL( OnAttachToScene, this, pGameObject );
		HG_LOG_INFO( std::string( "attached [" ).append( pGameObject->GetName() ).append( "] to scene: " ).append( GetName() ).c_str() );
	} else {
		Log->Warning( SDL_LOG_CATEGORY_SYSTEM, std::string( "trying to attach a same name game object, which will do not recover the original one. Name: " ).append( pGameObject->GetName() ).c_str() );
	}
}

GameObject* Scene::FindGameObject( const char* strName ) {
	return umGameObjectsByName.count( strName ) == 0 ? nullptr : umGameObjectsByName[strName];
}

void __HGImpl::V1SDL::Scene::Update( void* pEvent ) {
	static bool IsStart = false;
	if( !IsStart ) {
		for( auto& it : umGameObjectsByName ) {
			if( it.second->IsEnable() ) {
				HG_EVENT_CALL( Start, pEvent, it.second );
			}
		}
		IsStart = true;
	}
	for( auto& it : umGameObjectsByName ) {
		if( it.second->IsEnable() ) {
			it.second->Update( pEvent );
		}
	}
}

void __HGImpl::V1SDL::Scene::Render( void* pRenderer ) {
	for( auto& it : umGameObjectsByName ) {
		if( it.second->IsEnable() ) {
			HG_EVENT_CALL( OnUpdate, &HGMainLoop::tEvent, it.second );
			it.second->Render( pRenderer );
			HG_EVENT_CALL( OnPostRender, pRenderer, it.second );
		}
	}
}
