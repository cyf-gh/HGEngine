#pragma once

#include "HGEvent.h"
#include "HGComponent.h"

namespace HG {
/// \brief 
/// ��Ϊ�����GameObjectĬ�ϴ��������������������Ҫ��Scene����<br>
/// behavior component, which is created by game object by default. events' life cycles are controlled by Scene
/// \see __HGImpl::V1SDL::GameObject
/// \sa __HGImpl::V1SDL::Scene
class HGBehaviour : public HGComponent {
public:	
	/// \brief 
	/// �����¼���ÿ�̶�ʱ�����<br>
	/// update per fixed time
	pEvent OnFixedUpdate;
	/// \brief 
	///	�����¼���ÿ֡���£���OnRenderͬ��<br>
	///	update per frame, sync with OnRender
	/// \note
	/// * �÷�����Scene���ã���Ӧ����GameObject::Render����<br>
	/// this event is invoked by scene, you should not call it in GameObject::Render
	///	* GameObject���ڵ���OnRender֮ǰ���ø÷���<br>
	/// GameObject will invoke this event before OnRender
	/// * Scene���ڵ���OnRender֮ǰ���ø÷���<br>
	/// Scene will invoke this event before OnRender
	/// \see __HGImpl::V1SDL::Scene::Render
	pEvent OnUpdate;
	/// \brief 
	///	��Ⱦ�¼�<br>
	/// render event
	/// \see __HGImpl::V1SDL::Scene::Render
	pEvent OnRender;

	pEvent OnPostRender;

	pEvent OnAttach;

	pEvent OnEnable;

	pEvent OnDisable;

	/// \brief �ڶ��󱻹���ǰ������<br>
	/// invoke before object being constructed
	pEvent OnBeforeConstruct;

	/// \brief 
	/// ��Update�ĵ�һ֡������<br>
	/// invoked at first frame in Update
	/// \see __HGImpl::V1SDL::Scene::Update
	pEvent Start;

	/// \brief 
	/// ���������µ� Layer ǰʱ����
	/// \note 
	/// * pData �����ֵΪ LayerIndex
	/// \see __HGImpl::V1SDL::Layer::AttachGameObject
	pEvent OnAttachToLayer;
	
	/// \brief 
	/// ����ԭ���� Layer ���ǰ����
	/// \note 
	/// * pData �����ֵΪ LayerIndex
	/// \see __HGImpl::V1SDL::Layer::DetachGameObject
	pEvent OnDetachFromLayer;
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

#define HG_EVENT_ONRENDER_RENDERER ( static_cast< __HGImpl::V1SDL::Renderer2D* >( pData ) )