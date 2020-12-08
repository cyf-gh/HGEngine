////
//// Created by cyf-m on 2020/12/6.
////
//
//#ifndef HONEYGAME_EVENTPROC_H
//#define HONEYGAME_EVENTPROC_H
//
//#include <unordered_map>
//#include <SDL.h>
//#include "../Core/Type.h"
//
//namespace __HGImpl { namespace V1 {
//    typedef const HGCore::HGResult* (*FuncProc)(SDL_Event* , void *);
//    struct Event {
//    public:
//        FuncProc pFP;
//        std::string strName;
//    };
//    class EventProc {
//    protected:
//        std::unordered_map<Uint32,Event*> umEvents;
//        bool isEventExist(const Uint32 eType );
//    public:
//        void Invoke( SDL_Event *pE );
//        void AddEvent( const Uint32 eType, Event* pEvent );
//        EventProc();
//        virtual ~EventProc();
//    };
//} }
//#endif //HONEYGAME_EVENTPROC_H
