//
// Created by cyf-m on 2020/12/8.
//

#ifndef HONEYGAME_GAMEOBJECT_H
#define HONEYGAME_GAMEOBJECT_H

#include <typeinfo>
#include <string>
#include <unordered_map>
#include <vector>
#include "../Engine/HGObject.h"
#include "../Engine/HGComponent.h"
#include "../Engine/HGBehaviour.h"

namespace HGEngine {
namespace V1SDL {
class Scene;
class Layer;

/// \brief game object of HG
/// \note game object should be constructed after scenes;
/// if you don't want to attach to the current scene, you may construct the game objects before Engine
class GameObject : public HG::HGObject<GameObject> {
protected:
	Scene* m_pScene;
	Layer* m_pLayer;
	bool mIsEnable = false;
	std::vector<HG::HGComponent*> m_vecComponents;

public:

	virtual void Update( void* pEvent ) = 0;
	virtual void Render( void* pRenderer ) = 0;

	virtual GameObject* Clone() = 0;
	void Enable() { mIsEnable = true; HG_EVENT_CALL_NO_DATA( OnEnable, this ); }
	void Disable() { mIsEnable = false; HG_EVENT_CALL_NO_DATA( OnDisable, this ); }
	bool IsEnable() const { return mIsEnable; }
	
	/// \brief GameObject 是否于 Camera 的 View 之中
	/// \note
	/// * 该方法未使用 Collision 组件
	bool IsInCameraView();
	std::vector<HG::HGComponent*> GetAllComponents() const { return m_vecComponents; }
	HG::HGComponent* AddComponent( HG::HGComponent* pComp ) {
		m_vecComponents.push_back( static_cast< HG::HGComponent* >( pComp ) );
		static_cast< HG::HGComponent* >( pComp )->SetGameObject( this );
		return pComp;
	}
	template<typename T> std::vector<T*> GetComponents() { 
		std::vector<T*> m_vecComps;
		for( auto& c : m_vecComponents ) {
			if( typeid(T) == typeid(*c) ) {
				m_vecComps.push_back( c );
			}
		}
		return m_vecComps;
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
			if( ( strcmp( c->GetName(), strName ) == 0 ) ) {
				return static_cast< T* >( c );
			}
		}
		return nullptr;
	}
	Scene* GetScene() const { return m_pScene; }
	void SetScene( Scene* pS ) { m_pScene = pS; }
	Layer* GetLayer() const { return m_pLayer; }
	void SetLayer( Layer* pL ) { m_pLayer = pL; }

	/// \brief default's, the game object will be attached to the current scene
	explicit GameObject( const char* strName, Scene* pScene = nullptr );
	virtual ~GameObject();
};
}
}

#define HG_EVENT_THIS_GAMEOBJECT static_cast<GameObject *>(pThis)

#endif //HONEYGAME_GAMEOBJECT_H
