//
// Created by cyf-m on 2020/12/8.
//

#ifndef HONEYGAME_SCENEMAINGAME_H
#define HONEYGAME_SCENEMAINGAME_H

#include <SDL_events.h>
#include "Core/Log.h"
#include "EngineImpl/Scene.h"

using namespace __HGImpl::V1;

class SceneMainGame : Scene {
public:
    void Update( void* pe ) override {
        auto pEvent = static_cast<SDL_Event *>( pe );
        switch ( pEvent->type ) {
            case SDL_KEYDOWN:
                HG_LOG_INFO( SDL_GetKeyName( pEvent->key.keysym.sym ) );
                break;
            case SDL_QUIT:
                HG_LOG_INFO( "bye!!!" );
                SDL_Quit();
                exit(0); // break;
        }
    }
    void OnEnable() override {

    }
    void OnDisable() override {

    }
    void Render( HGCore::Renderer *pRenderer ) override {
    }
    explicit SceneMainGame( const char* strName ) : Scene( strName ) {

    }
};


#endif //HONEYGAME_SCENEMAINGAME_H
