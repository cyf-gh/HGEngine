//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_SCENE_H
#define HONEYGAME_SCENE_H

#include "Object.h"
#include "GameObject.h"
#include <unordered_map>

namespace __HGImpl { namespace V1 {

        /// \brief
        /// Scene::OnAttach will be invoked in EngineImpl::NavigateScene
        /// \note all scene should be constructed before the construction of Engine
        class Scene : public GameObject {
        protected:
            std::unordered_map<std::string, GameObject*> umGameObjectsByName;
            void updateAllGameObjects( void *pEv );
            void renderAllGameObjects( HGCore::Renderer* pRd );

        public:
            /// \brief 将一个GameObject加入场景 （attach a new game object to scene）
            /// \note 
            /// * 同名GameObject并不会覆盖原有的GameObject （game object which has a duplicated name will not recover the old one）
            /// * 该方法不应该由用户调用，GameObject的构造将自动调用该方法 （ this method should NOT be invoked by user; it will be invoked in the constructor of GameObject）
            /// \sa GameObject::GameObject
            void AttachGameObject( GameObject* pGameObject );
            /// \brief find game object by name
            /// \return nullptr when the specified game object does not exist
            GameObject* FindGameObject( const char* strName );

            void Update( void* pEvent ) override { }
            void Render( HGCore::Renderer* pRenderer ) override { }
            void OnAttach() override { }
            void OnEnable() override { }
            void OnDisable() override { }
            
            explicit Scene(const char* strName);
            virtual ~Scene();
        };
} }

#endif //HONEYGAME_SCENE_H
