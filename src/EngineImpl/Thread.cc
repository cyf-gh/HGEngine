//
// Created by cyf-m on 2020/11/28.
//

#include <SDL_log.h>
#include <string>
#include <Log.h>
#include "Thread.h"

using namespace std;
using namespace HGEngine::V1SDL;
using namespace HG;

std::unordered_map<const char*, Thread*> Thread::Threads = {};

Thread::Thread( SDL_ThreadFunction pf, const char* strThreadName, void* pData, bool isAwait )
	: IsAwait( isAwait ), strName( strThreadName ) {
	pHandle = SDL_CreateThread( pf, strThreadName, pData );
	HG_LOG_CHECK_SDL_HANDLE_IS_NULL( pHandle, SDL_LOG_CATEGORY_SYSTEM, "Thread::Thread" );

	Log->Info( SDL_LOG_CATEGORY_SYSTEM, std::format( "Thread({}) =>> {}", strThreadName, isAwait ? "AWAIT" : "ASYNC" ).c_str() );

	if( isAwait ) {
		int status = 0;
		SDL_WaitThread( pHandle, &status );
		string strLogInfoFinished = strThreadName;
		strLogInfoFinished.append( " finished with code: " ).append( to_string( status ) );
		Log->Info( SDL_LOG_CATEGORY_SYSTEM, strLogInfoFinished.c_str() );
	} else {
		Threads[GetName()] = this;
		SDL_DetachThread( pHandle );
	}
}

Thread::~Thread() {
	Threads[GetName()] = nullptr;
	HG_LOG_INFO( std::format( "Thread({}) ~Destructed", GetName() ).c_str() );
}

HGResult Thread::SetPriority( SDL_ThreadPriority priority ) {
	if( 0 == SDL_SetThreadPriority( priority ) ) {
		HG_LOG_SDL_ERROR( SDL_LOG_CATEGORY_SYSTEM, "Thread::SetPriority" );
		return HG_ERR_THREAD;
	} else {
		return HG_ERR_OK;
	}
}

