//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_SCENE_H
#define HONEYGAME_SCENE_H

#include "Object.h"
#include <unordered_map>

namespace __HGImpl { namespace V1 {
        class GameObject;
        class Scene : public Object<Scene> {
        protected:
            std::unordered_map<const char*, GameObject*> umGameObjectsByName;
            void updateAllGameObjects( void *pEv );
            void renderAllGameObjects( HGCore::Renderer* pRd );

        public:
            /// \brief attach a new game object to scene
            /// \note game object which has a duplicated name will not recover the old one
            void AttachGameObject( GameObject* pGameObject );
            GameObject* FindGameObject( const char* strName );

            explicit Scene(const char* strName);
            virtual ~Scene();
        };
} }

#endif //HONEYGAME_SCENE_H
