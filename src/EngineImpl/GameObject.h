//
// Created by cyf-m on 2020/12/8.
//

#ifndef HONEYGAME_GAMEOBJECT_H
#define HONEYGAME_GAMEOBJECT_H

#include <string>
#include <unordered_map>
#include <vector>
#include "../Engine/HGBehaviour.h"
#include "../Engine/HGObject.h"
#include "../Engine/HGBehaviour.h"
#include "../Engine/HGComponent.h"

namespace __HGImpl {
namespace V1SDL {
class Scene;
/// \brief game object of HG
/// \note game object should be constructed after scenes;
/// if you dont want to attach to the current scene, you may construct the game objects before Engine
class GameObject : public HG::V1SDL::HGObject<GameObject>, public HG::V1SDL::HGBehaviour {
protected:
	Scene* m_pScene;
	bool mIsEnable = false;
	std::vector<HG::V1SDL::HGComponent*> m_vecComponents;

public:
	void Enable() { mIsEnable = true; HG_EVENT_CALL_NO_DATA( OnEnable, this ); }
	void Disable() { mIsEnable = false; HG_EVENT_CALL_NO_DATA( OnDisable, this ); }
	bool IsEnable() const { return mIsEnable; }
	
	bool IsInCameraView();
	void Update( void* pEvent ) override { };
	void Render( void* pRenderer ) override { };

	std::vector<HG::V1SDL::HGComponent*> GetComponents() const { return m_vecComponents; }
	HG::V1SDL::HGComponent* AddComponent( HG::V1SDL::HGComponent* pComp ) {
		m_vecComponents.push_back( static_cast< HG::V1SDL::HGComponent* >( pComp ) );
		static_cast< HG::V1SDL::HGComponent* >( pComp )->SetGameObject( this );
		return pComp;
	}

	template<typename T> T* GetComponent() {
		for( auto& c : m_vecComponents ) {
			if( typeid(T) == typeid(*c) ) {
				return static_cast< T* >( c );
			}
		}
		return nullptr;
	}
	template<typename T> T* GetComponent( const char* strName ) {
		for( auto& c : m_vecComponents ) {
			if( typeid(T) == typeid(*c) && ( strcmp( c->GetName(), strName ) == 0 ) ) {
				return static_cast< T* >( c );
			}
		}
		return nullptr;
	}
	/// \brief scene that the GameObject is part of.
	Scene* GetScene() const { return m_pScene; }
	/// \brief default's, the game object will be attached to the current scene
	explicit GameObject( const char* strName, Scene* pScene = nullptr );
	virtual ~GameObject();
};
}
}

#define HG_EVENT_THIS_GAMEOBJECT static_cast<GameObject *>(pThis)

#endif //HONEYGAME_GAMEOBJECT_H
