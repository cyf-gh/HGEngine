#pragma once

#include "HGEvent.hpp"
#include "HGComponent.h"

namespace HG {
/// \brief 
/// 行为组件，GameObject默认创建该组件，生命周期主要由Scene控制<br>
/// behavior component, which is created by game object by default. events' life cycles are controlled by Scene
/// \see HGEngine::V1SDL::GameObject
/// \sa HGEngine::V1SDL::Scene
class HGBehaviour : public HGComponent {
public:	
	/// \brief 
	/// 更新事件，每固定时间更新<br>
	/// \note
	/// * pData 为 SDL_Event
	/// update per fixed time
	pEvent OnFixedUpdate;
	/// \brief 
	///	更新事件，每帧更新，与OnRender同步<br>
	///	update per frame, sync with OnRender
	/// \note
	/// * pData 为 SDL_Event
	/// * 该方法由Scene调用，不应当由GameObject::Render调用<br>
	/// this event is invoked by scene, you should not call it in GameObject::Render
	///	* GameObject将于调用OnRender之前调用该方法<br>
	/// GameObject will invoke this event before OnRender
	/// * Scene将于调用OnRender之前调用该方法<br>
	/// Scene will invoke this event before OnRender
	/// \see HGEngine::V1SDL::Scene::Render
	pEvent OnUpdate;
	/// \brief 
	///	渲染事件<br>
	/// render event
	/// \note 
	/// * pData 为 HGEngine::V1SDL::Render2D
	/// * 在 renderCameraView 调用之前被调用
	/// \see HGEngine::V1SDL::Scene::Render
	pEvent OnRender;
	/// \brief 
	///	渲染后被调用事件<br>
	/// post render event
	/// \note 
	/// * pData 为 HGEngine::V1SDL::Render2D
	/// \see HGEngine::V1SDL::Scene::Render
	pEvent OnPostRender;
	/// \brief 
	///	当附加于新的 Scene 前时调用<br>
	/// \note 
	/// * pData 为 Scene
	/// \see HGEngine::V1SDL::Scene::AttachGameObject
	pEvent OnAttachToScene;
	/// \brief 
	///	被激活时调用<br>
	/// \note 
	/// * pData 为 nullptr
	/// \see HGEngine::V1SDL::GameObject::Enable
	pEvent OnEnable;
	/// \brief 
	///	被关闭激活时调用<br>
	/// \note 
	/// * pData 为 nullptr
	/// \see HGEngine::V1SDL::GameObject::Disable
	pEvent OnDisable;
	/// \brief 于对象被构造前被调用<br>
	/// \note
	/// * pData 为 SDL_Event
	/// invoke before object being constructed
	pEvent OnBeforeConstruct;
	/// \brief 
	/// 于Update的第一帧被调用<br>
	/// invoked at first frame in Update
	/// \note
	/// * pData 为 SDL_Event
	/// \see HGEngine::V1SDL::Scene::Update
	pEvent Start;
	/// \brief 
	/// 当附加于新的 Layer 前时调用
	/// \note 
	/// * pData 传入的值为 LayerIndex
	/// \see HGEngine::V1SDL::Layer::AttachGameObject
	pEvent OnAttachToLayer;
	/// \brief 
	/// 当从原本的 Layer 解除前调用
	/// \note 
	/// * pData 传入的值为 LayerIndex
	/// \see HGEngine::V1SDL::Layer::DetachGameObject
	pEvent OnDetachFromLayer;
	/// \brief 
	/// 当与另一个 GameObject 第一次接触时调用
	/// \note 
	/// * pData 为 另一个GameObject
	/// \see HGEngine::V1SDL::Collision::procCollided
	pEvent OnCollisionEnter;
	/// \brief 
	/// 当与另一个 GameObject 持续接触时调用
	/// \note 
	/// * pData 为 另一个GameObject
	/// \see HGEngine::V1SDL::Collision::procCollided
	pEvent OnCollisionStay;
	/// \brief 
	/// 当与另一个 GameObject 原本接触但离开时
	/// \note 
	/// * pData 为 另一个GameObject
	/// \see HGEngine::V1SDL::Collision::procCollided
	pEvent OnCollisionExit;

public:
	HGBehaviour( const char* strName ) : HGComponent( strName ) { }
	virtual ~HGBehaviour() {}
};
}

#define HG_EVENT_BIND( GAMEOBJECT, EVENT ) GAMEOBJECT->GetComponent<HG::HGBehaviour>()->EVENT =  []( void *pData, void *pThis ) -> int

#define HG_EVENT_ONUPDATE() []( void *pData, void *pThis ) -> int

#define HG_EVENT_ONUPDATE_EVENT ( static_cast< SDL_Event* >( pData ) )

#define HG_EVENT_ONUPDATE_ISKEY( KEY ) if ( HG_EVENT_ONUPDATE_EVENT->key.keysym.sym == KEY )

#define HG_EVENT_ONRENDER() []( void *pData, void *pThis ) -> int

#define HG_EVENT_ONRENDER_RENDERER ( static_cast< HGEngine::V1SDL::Renderer2D* >( pData ) )

#define HG_EVENT_IMPL []( void *pData, void *pThis ) -> int
