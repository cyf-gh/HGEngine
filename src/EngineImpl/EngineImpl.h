/// \file EngineImpl.h
/// \brief Engine implementation

#ifndef HONEYGAME_ENGINEIMPL_H
#define HONEYGAME_ENGINEIMPL_H

#include "../Engine/HG.h"
#include "Loop.h"
#include "Window.h"
#include "Thread.h"
#include "Camera.h"
#include "Renderer2D.h"

namespace HGEngine {
namespace V1SDL {
class Scene;
class Asset;
/// \brief main loop of engine
/// \details main loop is a event loop main thread
/// , faster is better
/// \sa EngineImpl::GetMainLoop()
class HGMainLoop : public Loop {
public:
	static SDL_Event tEvent;
	void _RunTask() override;
	void _PaddingTask() override;
	void _StopTask() override;
};
/// \brief update loop of engine
/// \sa EngineImpl::GetUpdateLoop()
class HGUpdateLoop : public Loop {
public:
	void _RunTask() override;
	void _PaddingTask() override;
	void _StopTask() override;
};
/// \brief render loop of engine
/// \sa EngineImpl::GetRenderLoop()
class HGRenderLoop : public Loop {
public:
	void _RunTask() override;
	void _PaddingTask() override;
	void _StopTask() override;
};

/// \brief HoneyGame主引擎 <br>engine of HG
/// \note 
/// * 初始化顺序应当为 <br>the initialization order of Scenes, GameObjects, Engine Should be
/// * Scenes -> Engine -> GameObjects
class EngineImpl : public HG::IEngine {
private:
	HGMainLoop tLoopMain;
	HGUpdateLoop tLoopUpdate;
	HGRenderLoop tLoopRender;
	HGEngine::V1SDL::Thread* pUpdateThread {};
	HGEngine::V1SDL::Thread* pRenderThread {};

	HGEngine::V1SDL::Window* pWindow;

	Renderer2D* pRenderer;
	Asset* pAsset;
	Scene* pCurrentScene;
	static void SetEngine( EngineImpl* pEngine ) { EngineImpl::pEngine = pEngine; }

public:
	/// \brief 获取主线程句柄 <br>get main loop handle
	/// \note
	/// 该循环运行于主线程之中
	const HGMainLoop& GetMainLoop() { return tLoopMain; }
	const HGUpdateLoop& GetUpdateLoop() { return tLoopUpdate; }
	const HGRenderLoop& GetRenderLoop() { return tLoopRender; }

	Scene* GetCurrentScene() { return pCurrentScene; }
	void NavigateScene( const char* strSceneName );
	static EngineImpl* GetEngine() { return pEngine; }
	static EngineImpl* pEngine;
	
	Asset* GetAssetManager() const { return pAsset; }
	float GetFixedUpdateTimeDelta() const { return tLoopMain.DeltaTime(); }
	HGEngine::V1SDL::Window* GetWindow() const { return pWindow; }
	Renderer2D* GetRenderer2D() const { return pRenderer; }

	void Exit();

	int Run() override;

	explicit EngineImpl( int argc, char** argv );
	virtual ~EngineImpl();
};

}
}
#define HG_ENGINE_TIMEDELTA EngineImpl::GetEngine()->GetFixedUpdateTimeDelta()
#define HG_ENGINE_FIND_GAMEOBJECT( NAME ) EngineImpl::GetEngine()->GetCurrentScene()->FindGameObject(NAME)

#endif //HONEYGAME_ENGINEIMPL_H
