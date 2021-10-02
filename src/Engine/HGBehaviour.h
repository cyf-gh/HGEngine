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
	
	/// \brief 每固定时间更新 （update per fixed time）
	pEvent OnFixedUpdate;
	/// \brief 每帧更新 （update per frame）
	/// \note
	///	* GameObject将于调用OnRender之前调用该方法，见Scene::renderAllGameObjects （GameObject will invoke this event before OnRender, see Scene::renderAllGameObjects）
	/// * Scene将于调用OnRender之前调用该方法，见Scene::Render （Scene will invoke this event before OnRender, see Scene::Render）
	pEvent OnUpdate;
	/// 
	pEvent OnRender;
	pEvent OnAttach;
	pEvent OnEnable;
	pEvent OnDisable;
};
}
}

#define HG_EVENT_ONUPDATE() []( void *pEventRaw, void *pThis ) -> int

#define HG_EVENT_ONUPDATE_EVENT ( static_cast< SDL_Event* >( pEventRaw ) )

#define HG_EVENT_ONUPDATE_ISKEY( KEY ) if ( HG_EVENT_ONUPDATE_EVENT->key.keysym.sym == KEY )

#define HG_EVENT_ONRENDER() []( void *pRendererRaw, void *pThis ) -> int

#define HG_EVENT_ONRENDER_RENDERER ( static_cast< __HGImpl::V1SDL::Renderer2D* >( pRenderer ) )