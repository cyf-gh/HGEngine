//
// Created by cyf-m on 2020/12/8.
//

#ifndef HONEYGAME_GAMEOBJECT_H
#define HONEYGAME_GAMEOBJECT_H

#include <string>
#include <unordered_map>
#include "Object.h"
namespace __HGImpl { namespace V1 {
        class Scene;
        /// \brief game object of HG
        /// \note game object should be constructed after scenes;
        /// if you dont want to attach to the current scene, you may construct the game objects before Engine
        class GameObject : public Object<GameObject> {
        private:
            Scene* m_pScene;
            bool mIsEnable = false;
        public:
            /// \brief will be invoked every
            /// \note use auto pEvent = static_cast<SDL_Event *>( pe ); to get the event
            virtual void Update( void* pEvent ) = 0;
            virtual void Render( HGCore::Renderer *pRenderer ) = 0;
            virtual void OnAttach() = 0;
            virtual void OnEnable() = 0;
            virtual void OnDisable() = 0;

            void Enable() { mIsEnable = true; OnEnable(); }
            void Disable() { mIsEnable = false; OnDisable(); }
            bool IsEnable() const { return mIsEnable; }

            /// \brief scene that the GameObject is part of.
            Scene* GetScene() const { return m_pScene; }
            /// \brief default's, the game object will be attached to the current scene
            explicit GameObject(const char* strName, Scene* pScene = nullptr, bool IsScene = false );
            virtual ~GameObject();
        };
} }

#endif //HONEYGAME_GAMEOBJECT_H
