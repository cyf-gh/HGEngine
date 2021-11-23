#include <Log.h>
#include "Scene.h"
#include "Animation.h"
#include "GameObject.h"
#include "EngineImpl.h"
#include "Timer.hpp"
#include "GUI.hpp"
#include "EventTrigger.hpp"

using namespace HGEngine::V1SDL;
using namespace HGEngine;
using namespace HG;
using namespace std;

Scene::Scene( const char* strName )
	: HGObject<Scene>( strName ), m_pMainCamera( nullptr ), m_vecLayers(), OnAttach( nullptr ) {
	for( int i = 0; i < HG_LAYER_LENGTH; ++i ) {
		m_vecLayers.push_back( new Layer( ( string( "Layer" ) + to_string( i ) ).c_str(), i ) );
	}
	HG_LOG_INFO( std::format( "Scene[{}] !Constructed", GetName() ).c_str() );
}

Scene::~Scene() {
	for( auto& it : umGameObjectsByName ) {
		HG_SAFE_DEL( it.second );
	}
	HG_LOG_INFO( std::format( "Scene[{}] ~Destructed", GetName() ).c_str() );
}

GUI* HGEngine::V1SDL::Scene::TryCreateGUI( const std::string& name, bool isVisiable ) {
	m_umGUIs[name] = new GUI( this, isVisiable );
	return m_umGUIs[name];
}

GUI* HGEngine::V1SDL::Scene::GetGUI( const std::string& name ) {
	return m_umGUIs[name];
}

void HGEngine::V1SDL::Scene::SetMainCamera( Camera* pCamera ) {
	umGameObjectsByName[pCamera->GetName()] = pCamera;
	m_pMainCamera = pCamera;
	m_vecLayers[HG_LAYER_INDEX::HG_LAYER_CAMERA]->AttachGameObject( pCamera );
}

void Scene::AttachGameObject( GameObject* pGameObject, char LayerIndex ) {
	if( umGameObjectsByName.count( pGameObject->GetName() ) == 0 ) {
		umGameObjectsByName[pGameObject->GetName()] = pGameObject;
		pGameObject->SetScene( this );
		if( LayerIndex >= HG_LAYER_0 && LayerIndex < HG_LAYER_LENGTH ) {
			m_vecLayers[LayerIndex]->AttachGameObject( pGameObject );
		} else {
			Log->Warning(
				SDL_LOG_CATEGORY_SYSTEM,
				std::format( "GameObject[{}] trying to attach to a layer which does not exsit", pGameObject->GetName() ).c_str() );
		}
		HG_EVENT_CALL( OnAttachToScene, this, pGameObject );
		HG_LOG_INFO( std::format( "GameObject[{}]<-Scene[{}] +Attached",
					 pGameObject->GetName(),
					 GetName()
		).c_str() );
	} else {
		Log->Warning( SDL_LOG_CATEGORY_SYSTEM, std::string( "trying to attach a same name game object, which will do not recover the original one. Name: " ).append( pGameObject->GetName() ).c_str() );
	}
}

GameObject* Scene::FindGameObject( const char* strName ) {
	return umGameObjectsByName.count( strName ) == 0 ? nullptr : umGameObjectsByName[strName];
}

void HGEngine::V1SDL::Scene::Update( void* pEvent ) {
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
			auto timers = it.second->GetComponents<Timer>();
			for( auto& t : timers ) {
				t->TikTok( HG_ENGINE_TIMEDELTA );
			}
			auto* et = it.second->GetComponent<EventTrigger>();
			if( et != nullptr ) {
				et->ProcEvents();
			}
		}
	}
	for( auto& it : m_vecLayers ) {
		it->DoCheck();
	}
	for( auto& gg : m_umGUIs ) {
		auto& g = gg.second;
		g->m_unUIIndex = 0;
		if( g->IsVisiable ) {
			HG_EVENT_CALLRAW_NO_DATA( g->OnGUI, g );
		}
	}
}

void HGEngine::V1SDL::Scene::Render( void* pRenderer ) {
	for( auto& it : umGameObjectsByName ) {
		if( it.second->IsEnable() && it.second != m_pMainCamera ) {
			HG_EVENT_CALL( OnUpdate, &HGMainLoop::tEvent, it.second );
			it.second->Render( pRenderer );
			HG_EVENT_CALL( OnPostRender, pRenderer, it.second );
		}
	}
	HG_EVENT_CALL( OnUpdate, &HGMainLoop::tEvent, m_pMainCamera );
	m_pMainCamera->Render( pRenderer );
	HG_EVENT_CALL( OnPostRender, pRenderer, m_pMainCamera );
}

