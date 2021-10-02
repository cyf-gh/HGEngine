//
// Created by cyf-m on 2020/12/9.
//

#ifndef HONEYGAME_EVENT_H
#define HONEYGAME_EVENT_H

namespace HG { namespace V1SDL {
typedef int ( *pEvent )( void* pData, void *pThis );
}
}

#define HG_EVENT_CALL_NO_DATA( EVENT, PTHIS ) \
	if ( EVENT != nullptr ) { EVENT( nullptr, PTHIS ); }

#define HG_EVENT_CALL( EVENT, PDATA, PTHIS ) \
	if ( EVENT != nullptr ) { EVENT( PDATA, PTHIS ); }

#define HG_EVENT_ISNULLPTR( EVENT ) \
	EVENT == nullptr

#define HG_EVENT_CALLABLE( EVENT ) \
	EVENT != nullptr

#endif //HONEYGAME_EVENT_H
