//
// Created by cyf-m on 2020/12/8.
//

#ifndef HONEYGAME_GAMEOBJECT_H
#define HONEYGAME_GAMEOBJECT_H

#include <typeinfo>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include "../Engine/HGObject.h"
#include "../Engine/HGComponent.h"
#include "../Engine/HGBehaviour.h"

namespace HGEngine {
namespace V1SDL {
class Scene;
class Layer;
class Renderer2D;
/// \brief game object of HG
/// \note game object should be constructed after scenes;
/// if you don't want to attach to the current scene, you may construct the game objects before Engine
class GameObject : public HG::HGObject<GameObject> {
protected:
	Scene* m_pScene;
	Layer* m_pLayer;
	bool mIsEnable = false;
	void renderCameraView( Renderer2D* pRenderer );

public:
	std::vector<HG::HGComponent*> m_vecComponents;

	virtual GameObject* Clone() { return new GameObject( *this ); };
	virtual void Update( void* pEvent );
	virtual void Render( void* pRenderer );

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

	template<typename T> std::vector<T*> GetComponents() const { 
		std::vector<T*> vecComps;
		for( auto& c : m_vecComponents ) {
			if( typeid(T).hash_code() == typeid(*c).hash_code() ) {
				vecComps.push_back( (T*)c );
			}
		}
		return vecComps;
	}
	std::vector<HG::HGComponent*> GetRenderableComponentsSorted() const {
		std::vector<HG::HGComponent*> vecRC;
		for( auto& c : m_vecComponents ) {
			if( c->IsRenderable() ) {
				vecRC.push_back( c );
			}
		}
		std::sort( vecRC.begin(), vecRC.end(), []( HG::HGComponent* c1, HG::HGComponent* c2 ) {
			return c1->nRenderIndex < c2->nRenderIndex;
		} );
		return vecRC;
	}

	template<typename T> T* GetComponent() const {
		for( auto& c : m_vecComponents ) {
			if( typeid(T).hash_code() == typeid(*c).hash_code() ) {
				return static_cast< T* >( c );
			}
		}
		return nullptr;
	}
	template<typename T> T* GetComponent( const char* strName ) const {
		for( auto& c : m_vecComponents ) {
			if( ( strcmp( c->GetName(), strName ) == 0 ) ) {
				return static_cast< T* >( c );
			}
		}
		return nullptr;
	}
	template<typename T> bool RemoveComponent() {
		int i = 0;
		for( auto& c : m_vecComponents ) {
			if( typeid( T ).hash_code() == typeid( *c ).hash_code() ) {
				m_vecComponents.erase( m_vecComponents.begin() + i );
				return true;
			}
			++i;
		}
		return false;
	}
	Scene* GetScene() const { return m_pScene; }
	void SetScene( Scene* pS ) { m_pScene = pS; }
	Layer* GetLayer() const { return m_pLayer; }
	void SetLayer( Layer* pL ) { m_pLayer = pL; }

	/// @brief 
	/// * 创建一个自带Transform Behavior Components，并附加至当前Scene的GameObject
	///	* will create Transform and Behavior defaultly
	/// @note 
	///	* 当直接在C++代码中创建GameObject时，请调用该构造函数
	/// @param strName 
	/// @param pScene 
	explicit GameObject( const char* strName, Scene* pScene = nullptr );
	/// @brief 创建一个仅携带Behavior的GameObject
	explicit GameObject();
	virtual ~GameObject();
};
}
}

#define HG_EVENT_THIS_GAMEOBJECT static_cast<GameObject *>(pThis)

#endif //HONEYGAME_GAMEOBJECT_H
