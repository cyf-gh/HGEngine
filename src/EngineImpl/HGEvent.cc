#include "../engine/HGEvent.hpp"
#include <../EngineImpl/Log.hpp>
using namespace HG;

static std::vector<HG::pEventNoArg>* p_vecEvents;

HG::InitBindingEventsImpl::InitBindingEventsImpl( pEventNoArg pE, bool isEnable ) {
	static std::vector<HG::pEventNoArg> g_vecEvents;
	p_vecEvents = &g_vecEvents;
	if( isEnable ) {
		g_vecEvents.push_back( pE );
	}
}

void HG::InitBindingEventsImpl::InvokeAll() {
	for( pEventNoArg& e : *p_vecEvents ) {
		if( e != nullptr ) {
			try {
				e();
			} catch(...) {
				HG_LOG_FAILED( "failed in InvokeAll" );
			}
		}
	}
}

