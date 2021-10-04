#pragma once

#ifndef HONEYGAME_EVENT_H
#define HONEYGAME_EVENT_H

namespace HG {
typedef int ( *pEvent )( void* pData, void* pThis );
typedef int ( *pEventNoArg )();

class HGInitBindingEvents {
public:
	virtual void InvokeAll() = 0;
};

class InitBindingEventsImpl : HGInitBindingEvents {
public:
	InitBindingEventsImpl( pEventNoArg pE );
	InitBindingEventsImpl();
	void InvokeAll() override;
};
}

#define HG_SCRIPT_START(SCRIPTNAME) static HG::InitBindingEventsImpl SCRIPTNAME( []() -> int {

#define HG_SCRIPT_END  return 0; } );

#define HG_SCRIPT_BINDALL() HG::InitBindingEventsImpl ____uniq____; ____uniq____.InvokeAll()

#define HG_EVENT_CALL_NO_DATA( EVENT, PTHIS ) \
	if ( PTHIS->GetComponent<HG::HGBehaviour>()->EVENT != nullptr ) { PTHIS->GetComponent<HG::HGBehaviour>()->EVENT( nullptr, PTHIS ); }

#define HG_EVENT_CALL_SCENE_NO_DATA( EVENT, PTHIS ) \
	if ( EVENT != nullptr ) { EVENT( nullptr, PTHIS ); }

#define HG_EVENT_CALL( EVENT, PDATA, PTHIS ) \
	if ( ( PTHIS->GetComponent<HG::HGBehaviour>() )->EVENT != nullptr ) { ( PTHIS->GetComponent<HG::HGBehaviour>() )->EVENT( (void *)PDATA, PTHIS ); }

#define HG_EVENT_ISNULLPTR( EVENT ) \
	this->GetComponent<HG::HGBehaviour>()->EVENT == nullptr

#define HG_EVENT_CALLABLE( EVENT ) \
	this->GetComponent<HG::HGBehaviour>()->EVENT != nullptr

#endif //HONEYGAME_EVENT_H
