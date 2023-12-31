#pragma once

#include <string>
#include <filesystem>
#include <Math.hpp>
#include "HGEvent.hpp"
#include "../engineImpl/EngineImpl.h"
#include "../engineImpl/Scene.h"
#include "../engineImpl/Collision.h"
#include "../engineImpl/RigidBody.h"
#include "../engineImpl/Animation.h"
#include "../engineImpl/EngineObjectSerilization.hpp"
#include "../engineImpl/Asset.h"
#include "../engineImpl/Timer.hpp"
#include "../engineImpl/Label.hpp"
#include "../engineImpl/Spirte.hpp"
#include "../engineImpl/GUI.hpp"
#include "../engineImpl/Geometry.hpp"
#include "../engineImpl/Effect.hpp"
#include "../engineImpl/EventTrigger.hpp"

using namespace HGEngine::V1SDL;
using namespace HG::Math;
using namespace std;

namespace HGX {

static const string& GetSceneSpecName( const char* strSceneName, const char* strGameObjectName ) {
	static string str;
	str = std::format( "{}_{}", strSceneName, strGameObjectName );
	return str;
}
#define HG_HGXGSSN( name ) HGX::GetSceneSpecName( strSceneName, name ).c_str()
#define HG_HGXGSSNT( name ) HGX::GetSceneSpecName( mStrName.c_str(), name ).c_str()

/// @brief 快速获得一个具有GUI层场景，具有Canvas Camera CameraEffect
/// @note 如果场景不存在则会被创建
/// @param strSceneName 场景名称
/// @param strGUIName GUI层名字
/// @return 场景
static Scene* GetSceneWithGUI( const char* strSceneName, const char* strGUIName = "Canvas" ) {
	auto* s = EngineImpl::FindScene( strSceneName );
	if( s == nullptr ) {
		s = new Scene( strSceneName );
	}
	// Canvas
	auto* canvas = s->TryCreateGUI( HG_HGXGSSN( "Canvas" ), true );
	// Camera
	Camera* pCamera = new Camera( HG_HGXGSSN( "MainCamera" ), s );
	pCamera->Enable();
	pCamera->SetCameraSizeToRendererSize();
	auto* e = new Effect( HG_HGXGSSN( "CameraEffect" ) );
	e->m_tSolidColor.Set( 0, 0, 0, 0 );
	pCamera->AddComponent( e );
	s->SetMainCamera( pCamera );
	return s;
}

}

