//
// Created by cyf-m on 2020/12/9.
//

#ifndef HONEYGAME_EVENT_H
#define HONEYGAME_EVENT_H

namespace HG { namespace V1SDL {
typedef int ( *pEvent )( void* pData );
}
}

#define HG_EVENT_CALL_NO_DATA( EVENT ) \
	if ( EVENT != nullptr ) { EVENT( nullptr ); }

#define HG_EVENT_CALL( EVENT, PDATA ) \
	if ( EVENT != nullptr ) { EVENT( PDATA ); }


#endif //HONEYGAME_EVENT_H
