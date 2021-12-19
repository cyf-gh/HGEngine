#pragma once

#ifndef HONEYGAME_EVENT_H
#define HONEYGAME_EVENT_H

#include <Memory.h>
#include <vector>

namespace HG {
typedef int ( *pEvent )( void* pData, void* pThis );
typedef int ( *pEventNoArg )();

class InitBindingEventsImpl {
public:
	/// @brief 
	/// @param pE 
	/// @param isEnable 脚本是否被启用
	InitBindingEventsImpl( pEventNoArg pE, bool isEnable = true );
	InitBindingEventsImpl() = default;
	void InvokeAll();
};
}

#define HG_SCRIPT_START(SCRIPTNAME) HG::InitBindingEventsImpl SCRIPTNAME( []() -> int {

/// \brief 不启用该脚本
#define HG_SCRIPT_END_DISABLE return 0; }, false );
#define HG_SCRIPT_END  return 0; } );

#define HG_SCRIPT_BINDALL() HG::InitBindingEventsImpl ____uniq____; ____uniq____.InvokeAll()

#define HG_EVENT_CALL_NO_DATA( EVENT, PTHIS ) \
	if ( PTHIS->GetComponent<HG::HGBehaviour>()->EVENT != nullptr ) { PTHIS->GetComponent<HG::HGBehaviour>()->EVENT( nullptr, PTHIS ); }

#define HG_EVENT_CALLRAW_NO_DATA( EVENT, PTHIS ) \
	if ( EVENT != nullptr ) { EVENT( nullptr, PTHIS ); }

#define HG_EVENT_CALL( EVENT, PDATA, PTHIS ) \
	if ( ( PTHIS->GetComponent<HG::HGBehaviour>() )->EVENT != nullptr ) { ( PTHIS->GetComponent<HG::HGBehaviour>() )->EVENT( (void *)PDATA, PTHIS ); }

#define HG_EVENT_ISNULLPTR( EVENT ) \
	this->GetComponent<HG::HGBehaviour>()->EVENT == nullptr

#define HG_EVENT_CALLABLE( EVENT ) \
	this->GetComponent<HG::HGBehaviour>()->EVENT != nullptr

#endif //HONEYGAME_EVENT_H
