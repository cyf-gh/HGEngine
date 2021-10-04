#pragma once

#include "HGEvent.h"
#include "HGComponent.h"

namespace HG {
class HGBehaviour : public HGComponent {
public:	
	/// \brief ÿ�̶�ʱ����� ��update per fixed time��
	pEvent OnFixedUpdate;
	/// \brief ÿ֡���� ��update per frame��
	/// \note
	/// * �������÷�����Scene���ã���Ӧ����GameObject::Render���ã�����
	///	* GameObject���ڵ���OnRender֮ǰ���ø÷�������Scene::renderAllGameObjects ��GameObject will invoke this event before OnRender, see Scene::renderAllGameObjects��
	/// * Scene���ڵ���OnRender֮ǰ���ø÷�������Scene::Render ��Scene will invoke this event before OnRender, see Scene::Render��
	pEvent OnUpdate;
	/// 
	pEvent OnRender;

	pEvent OnPostRender;

	pEvent OnAttach;

	pEvent OnEnable;

	pEvent OnDisable;

	/// \brief �ڶ��󱻹���ǰ������ ��invoke before object being constructed��
	pEvent OnBeforeConstruct;

	/// \brief ��Update�ĵ�һ֡������
	/// \note 
	/// * Scene::Start -> GameObject::Start
	/// * See Scene::Update
	pEvent Start;
	
public:
	HGBehaviour( const char* strName ) : HGComponent( strName ) {
		
	}
	virtual ~HGBehaviour() {}
};
}

#define HG_EVENT_BIND( GAMEOBJECT, EVENT ) GAMEOBJECT->GetComponent<HG::HGBehaviour>()->EVENT =  []( void *pData, void *pThis ) -> int

#define HG_EVENT_ONUPDATE() []( void *pData, void *pThis ) -> int

#define HG_EVENT_ONUPDATE_EVENT ( static_cast< SDL_Event* >( pData ) )

#define HG_EVENT_ONUPDATE_ISKEY( KEY ) if ( HG_EVENT_ONUPDATE_EVENT->key.keysym.sym == KEY )

#define HG_EVENT_ONRENDER() []( void *pData, void *pThis ) -> int

#define HG_EVENT_ONRENDER_RENDERER ( static_cast< __HGImpl::V1SDL::Renderer2D* >( pData ) )