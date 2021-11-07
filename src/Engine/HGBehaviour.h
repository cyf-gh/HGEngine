#pragma once

#include "HGEvent.hpp"
#include "HGComponent.h"

namespace HG {
/// \brief 
/// ��Ϊ�����GameObjectĬ�ϴ��������������������Ҫ��Scene����<br>
/// behavior component, which is created by game object by default. events' life cycles are controlled by Scene
/// \see HGEngine::V1SDL::GameObject
/// \sa HGEngine::V1SDL::Scene
class HGBehaviour : public HGComponent {
public:	
	/// \brief 
	/// �����¼���ÿ�̶�ʱ�����<br>
	/// \note
	/// * pData Ϊ SDL_Event
	/// update per fixed time
	pEvent OnFixedUpdate;
	/// \brief 
	///	�����¼���ÿ֡���£���OnRenderͬ��<br>
	///	update per frame, sync with OnRender
	/// \note
	/// * pData Ϊ SDL_Event
	/// * �÷�����Scene���ã���Ӧ����GameObject::Render����<br>
	/// this event is invoked by scene, you should not call it in GameObject::Render
	///	* GameObject���ڵ���OnRender֮ǰ���ø÷���<br>
	/// GameObject will invoke this event before OnRender
	/// * Scene���ڵ���OnRender֮ǰ���ø÷���<br>
	/// Scene will invoke this event before OnRender
	/// \see HGEngine::V1SDL::Scene::Render
	pEvent OnUpdate;
	/// \brief 
	///	��Ⱦ�¼�<br>
	/// render event
	/// \note 
	/// * pData Ϊ HGEngine::V1SDL::Render2D
	/// * �� renderCameraView ����֮ǰ������
	/// \see HGEngine::V1SDL::Scene::Render
	pEvent OnRender;
	/// \brief 
	///	��Ⱦ�󱻵����¼�<br>
	/// post render event
	/// \note 
	/// * pData Ϊ HGEngine::V1SDL::Render2D
	/// \see HGEngine::V1SDL::Scene::Render
	pEvent OnPostRender;
	/// \brief 
	///	���������µ� Scene ǰʱ����<br>
	/// \note 
	/// * pData Ϊ Scene
	/// \see HGEngine::V1SDL::Scene::AttachGameObject
	pEvent OnAttachToScene;
	/// \brief 
	///	������ʱ����<br>
	/// \note 
	/// * pData Ϊ nullptr
	/// \see HGEngine::V1SDL::GameObject::Enable
	pEvent OnEnable;
	/// \brief 
	///	���رռ���ʱ����<br>
	/// \note 
	/// * pData Ϊ nullptr
	/// \see HGEngine::V1SDL::GameObject::Disable
	pEvent OnDisable;
	/// \brief �ڶ��󱻹���ǰ������<br>
	/// \note
	/// * pData Ϊ SDL_Event
	/// invoke before object being constructed
	pEvent OnBeforeConstruct;
	/// \brief 
	/// ��Update�ĵ�һ֡������<br>
	/// invoked at first frame in Update
	/// \note
	/// * pData Ϊ SDL_Event
	/// \see HGEngine::V1SDL::Scene::Update
	pEvent Start;
	/// \brief 
	/// ���������µ� Layer ǰʱ����
	/// \note 
	/// * pData �����ֵΪ LayerIndex
	/// \see HGEngine::V1SDL::Layer::AttachGameObject
	pEvent OnAttachToLayer;
	/// \brief 
	/// ����ԭ���� Layer ���ǰ����
	/// \note 
	/// * pData �����ֵΪ LayerIndex
	/// \see HGEngine::V1SDL::Layer::DetachGameObject
	pEvent OnDetachFromLayer;
	/// \brief 
	/// ������һ�� GameObject ��һ�νӴ�ʱ����
	/// \note 
	/// * pData Ϊ ��һ��GameObject
	/// \see HGEngine::V1SDL::Collision::procCollided
	pEvent OnCollisionEnter;
	/// \brief 
	/// ������һ�� GameObject �����Ӵ�ʱ����
	/// \note 
	/// * pData Ϊ ��һ��GameObject
	/// \see HGEngine::V1SDL::Collision::procCollided
	pEvent OnCollisionStay;
	/// \brief 
	/// ������һ�� GameObject ԭ���Ӵ����뿪ʱ
	/// \note 
	/// * pData Ϊ ��һ��GameObject
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
