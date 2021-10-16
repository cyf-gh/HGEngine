//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_THREAD_H
#define HONEYGAME_THREAD_H

#include <unordered_map>
#include <SDL_thread.h>

#include <Memory.h>
#include <Error.h>

namespace __HGImpl {
namespace V1SDL {
/// \brief wrapped SDL_Thread
/// \note await thread should not be used in main thread to avoid UI blocking.
class Thread : HG::Memory::NonCopyable {
private:
	std::string strName;
public:
	/// \brief threads map
	/// \note await thread will not be attached here; async thread won't become nullptr until Thread
	/// is destructed
	/// \sa Thread::~Thread()
	static std::unordered_map<const char*, Thread*> Threads;
	/// \brief set current thread priority
	static HG::HGResult SetPriority( SDL_ThreadPriority priority );
	
public:
	SDL_Thread* pHandle;
	const bool IsAwait;
	const char* GetName() const { return strName.c_str(); }
	Thread( SDL_ThreadFunction pf, const char* strThreadName, void* pData = nullptr, bool isAwait = true );
	virtual ~Thread();
};

}
}

#endif //HONEYGAME_THREAD_H
