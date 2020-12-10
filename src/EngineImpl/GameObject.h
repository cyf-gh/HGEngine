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

        public:
            /// \brief scene that the GameObject is part of.
            Scene* GetScene() const { return m_pScene; }
            /// \brief default's, the game object will be attached to the current scene
            explicit GameObject(const char* strName, Scene* pScene = nullptr );
            virtual ~GameObject();
        };
} }

#endif //HONEYGAME_GAMEOBJECT_H
