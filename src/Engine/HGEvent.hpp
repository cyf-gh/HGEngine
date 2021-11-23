#pragma once

#ifndef HONEYGAME_EVENT_H
#define HONEYGAME_EVENT_H

#include <Memory.h>
#include <vector>

namespace HG {
typedef int ( *pEvent )( void* pData, void* pThis );
typedef int ( *pEventNoArg )();

class HGInitBindingEvents {
public:
	virtual void InvokeAll() = 0;
};

class InitBindingEventsImpl : HGInitBindingEvents {
public:
	static std::vector<pEventNoArg> m_vecEvents;
	static std::vector<pEventNoArg> m_vecEventsDisabled;
	/// @brief 
	/// @param pE 
	/// @param isEnable 脚本是否被启用
	InitBindingEventsImpl( pEventNoArg pE, bool isEnable = true ) {
		if( isEnable ) {
			m_vecEvents.push_back( pE );
		} else {
			m_vecEventsDisabled.push_back( pE );
		}
	}
	InitBindingEventsImpl() = default;
	void InvokeAll() override {
		for( pEventNoArg& e : m_vecEvents ) {
			if( e != nullptr ) {
				e();
			}
		}
	}
};
}

#define HG_SCRIPT_INIT std::vector<HG::pEventNoArg> HG::InitBindingEventsImpl::m_vecEvents = std::vector<HG::pEventNoArg>(); std::vector<HG::pEventNoArg> HG::InitBindingEventsImpl::m_vecEventsDisabled = std::vector<HG::pEventNoArg>();

#define HG_SCRIPT_START(SCRIPTNAME) static HG::InitBindingEventsImpl SCRIPTNAME( []() -> int {

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
