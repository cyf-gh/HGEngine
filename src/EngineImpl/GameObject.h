//
// Created by cyf-m on 2020/12/8.
//

#ifndef HONEYGAME_GAMEOBJECT_H
#define HONEYGAME_GAMEOBJECT_H

#include <string>
#include <unordered_map>
#include "Object.h"
#include "EngineImpl.h"

namespace __HGImpl { namespace V1 {
        class Scene;
        class GameObject : public Object<GameObject> {
        private:
            Scene* m_pScene;

        public:
            /// \brief scene that the GameObject is part of.
            Scene* GetScene() const { return m_pScene; }
            void SetScene( Scene* pScene ) { m_pScene = pScene; }
            explicit GameObject(const char* strName);
            virtual ~GameObject();
        };
} }

#endif //HONEYGAME_GAMEOBJECT_H
