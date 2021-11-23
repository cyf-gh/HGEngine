#pragma once

#include <string>
#include <filesystem>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
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

/// @brief ���ٻ��һ������GUI�㳡��
/// @note ���������������ᱻ����
/// @param strSceneName ��������
/// @param strGUIName GUI������
/// @return ����
static Scene* GetSceneWithGUI( const char* strSceneName, const char* strGUIName = "Canvas" ) {
	auto* s = EngineImpl::FindScene( strSceneName );
	if( s == nullptr ) {
		s = new Scene( strSceneName );
	}
	auto* canvas = s->TryCreateGUI( "Canvas", true );
	return s;
}

}
