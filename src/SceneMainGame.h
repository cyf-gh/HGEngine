//
// Created by cyf-m on 2020/12/8.
//

#ifndef HONEYGAME_SCENEMAINGAME_H
#define HONEYGAME_SCENEMAINGAME_H

#include <SDL_events.h>
#include "Core/Log.h"
#include "EngineImpl/Scene.h"

using namespace __HGImpl::V1;

class SceneMainGame : public Scene {
public:
	void Update( void* pe ) override {
		auto pEvent = static_cast< SDL_Event* >( pe );
		updateAllGameObjects( pEvent );
		switch( pEvent->type ) {
		case SDL_KEYDOWN:
		HG_LOG_INFO( SDL_GetKeyName( pEvent->key.keysym.sym ) );
		HG_LOG_INFO( std::to_string( EngineImpl::GetEngine()->GetMainLoop().GetCurrentFps() ).append( " <- main loop fps" ).c_str() );
		HG_LOG_INFO( std::to_string( EngineImpl::GetEngine()->GetRenderLoop().GetCurrentFps() ).append( " <- render loop fps" ).c_str() );
		HG_LOG_INFO( std::to_string( EngineImpl::GetEngine()->GetUpdateLoop().GetCurrentFps() ).append( " <- update loop fps" ).c_str() );
		break;
		case SDL_QUIT:
		HG_LOG_INFO( "bye!!!" );
		EngineImpl::GetEngine()->Exit();
		return;
		break;
		}
	}
	void OnEnable() override {

	}
	void OnDisable() override {

	}
	void OnAttach() override {

	}
	void Render( HGCore::Renderer* pRenderer ) override {
		static int i = 0;
		GameObjectText* tFpsText = static_cast< GameObjectText* >( GameObject::Find( "test_fps" ) );
		if( tFpsText != nullptr ) {
			auto str = std::to_string( EngineImpl::GetEngine()->GetRenderLoop().GetCurrentFps() );
			str.resize( 3 );
			str += + " | " + std::to_string(i);
			i++;
			tFpsText->Text = str;
		}
		renderAllGameObjects( pRenderer );
	}
	explicit SceneMainGame( const char* strName ) : Scene( strName ) {

	}
	virtual ~SceneMainGame() = default;
};


#endif //HONEYGAME_SCENEMAINGAME_H
