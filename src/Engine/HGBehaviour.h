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

	pEvent OnUpdate;
	pEvent OnRender;
	pEvent OnAttach;
	pEvent OnEnable;
	pEvent OnDisable;
};
}
}

#define HG_EVENT_ONUPDATE() []( void *pEventRaw ) -> int

#define HG_EVENT_ONUPDATE_EVENT ( static_cast< SDL_Event* >( pEventRaw ) )

#define HG_EVENT_ONRENDER() []( void *pRendererRaw ) -> int

#define HG_EVENT_ONRENDER_RENDERER ( static_cast< __HGImpl::V1SDL::Renderer2D* >( pRenderer ) )