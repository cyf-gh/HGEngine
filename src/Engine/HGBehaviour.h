#pragma once

#include "HGEvent.h"

namespace HG {
namespace V1SDL {
class HGBehaviour {
public:
	/// \brief will be invoked every
	/// \note use auto pEvent = static_cast<SDL_Event *>( pe ); to get the event
	virtual void Update( void* pEvent ) = 0;
	virtual void Render( void* pRenderer ) = 0;
	
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
};
}
}

#define HG_EVENT_ONUPDATE() []( void *pEventRaw, void *pThis ) -> int

#define HG_EVENT_ONUPDATE_EVENT ( static_cast< SDL_Event* >( pEventRaw ) )

#define HG_EVENT_ONUPDATE_ISKEY( KEY ) if ( HG_EVENT_ONUPDATE_EVENT->key.keysym.sym == KEY )

#define HG_EVENT_ONRENDER() []( void *pRendererRaw, void *pThis ) -> int

#define HG_EVENT_ONRENDER_RENDERER ( static_cast< __HGImpl::V1SDL::Renderer2D* >( pRenderer ) )