////
//// Created by cyf-m on 2020/12/6.
////
//
//#include "EventProc.h"
//#include "../Core/Log.h"
//
//using namespace __HGImpl::V1;
//using namespace HG;
//using namespace std;
//
//EventProc::EventProc() {
//
//}
//
//EventProc::~EventProc() {
//    for(auto& v : umEvents) {
//        HG_SAFE_DEL( v.second );
//    }
//}
//
//void EventProc::Invoke( SDL_Event *pE ) {
//    if ( isEventExist( pE->type ) ) {
//        auto pEvent = umEvents[ pE->type ];
//        pEvent->pFP( pE, nullptr );
//    }
//}
//
//bool EventProc::isEventExist(const Uint32 eType ) {
//    return umEvents.count(eType) == 1;
//}
//
//void EventProc::AddEvent( const Uint32 et, __HGImpl::V1::Event *pEvent) {
//    if ( isEventExist( et ) ) {
//        Log->Warning( SDL_LOG_CATEGORY_SYSTEM, std::string("trying to overwrite event in EventProc::AddEvent << " ).append( to_string( eType )).c_str() );
//        HG_SAFE_DEL( umEvents[et] );
//    }
//    umEvents[et] = pEvent;
//}
