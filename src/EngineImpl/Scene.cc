//
// Created by cyf-m on 2020/11/28.
//

#include "../Core/Log.h"
#include "Scene.h"
#include "Animation.h"
#include "GameObject.h"
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
	HG_LOG_INFO( std::format( "Scene[{}] ! Constructed", GetName() ).c_str() );
}

Scene::~Scene() {
	for( auto& it : umGameObjectsByName ) {
		HG_SAFE_DEL( it.second );
	}
	HG_LOG_INFO( std::format( "Scene[{}] ! Destructed", GetName() ).c_str() );
}

void Scene::AttachGameObject( GameObject* pGameObject, char LayerIndex ) {
	if( umGameObjectsByName.count( pGameObject->GetName() ) == 0 ) {
		umGameObjectsByName[pGameObject->GetName()] = pGameObject;
		if( LayerIndex >= HG_LAYER_0 && LayerIndex < HG_LAYER_LENGTH ) {
			m_vecLayers[LayerIndex]->AttachGameObject( pGameObject );
		} else {
			Log->Warning(
				SDL_LOG_CATEGORY_SYSTEM,
				std::format( "GameObject[{}] trying to attach to a layer which does not exsit", pGameObject->GetName() ).c_str() );
		}
		HG_EVENT_CALL( OnAttachToScene, this, pGameObject );
		HG_LOG_INFO( std::format( "Scene[{}]->GameObject[{}] ! Attached", GetName(), pGameObject->GetName() ).c_str() );
	} else {
		Log->Warning( SDL_LOG_CATEGORY_SYSTEM, std::string( "trying to attach a same name game object, which will do not recover the original one. Name: " ).append( pGameObject->GetName() ).c_str() );
	}
}

GameObject* Scene::FindGameObject( const char* strName ) {
	return umGameObjectsByName.count( strName ) == 0 ? nullptr : umGameObjectsByName[strName];
}

void __HGImpl::V1SDL::Scene::Update( void* pEvent ) {
	static bool IsStart = false;
	// 首次运行调用
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
	for( auto& it : m_vecLayers ) {
		it->DoCheck();
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
