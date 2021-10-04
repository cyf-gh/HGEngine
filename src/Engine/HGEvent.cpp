#include <vector>
#include "HGEvent.h"

using namespace HG;

static std::vector<pEventNoArg> m_vecEvents;

HG::InitBindingEventsImpl::InitBindingEventsImpl( pEventNoArg pE ) { m_vecEvents.push_back( pE ); }

HG::InitBindingEventsImpl::InitBindingEventsImpl() { }

void HG::InitBindingEventsImpl::InvokeAll() {
	for( pEventNoArg& e : m_vecEvents ) {
		if( e != nullptr ) {
			e();
		}
	}
}
