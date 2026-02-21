/// \file EngineImpl.h
/// \brief Engine implementation
#pragma once

#include "../Engine/HG.h"
#include "Log.hpp"
#include "Loop.h"
#include "Window.h"
#include "Thread.h"
#include "Physics.hpp"
#include "Renderer2D.h"
#include "AssetManager.h"
#include "Editor\Editor.h"

namespace HG{
class HGInput;
}

namespace HGEngine {
namespace V1SDL {
class Scene;
class Asset;
class AssetManager;
class Editor;
/// \brief main loop of engine
/// \details main loop is a event loop main thread
/// , faster is better
/// \sa EngineImpl::GetMainLoop()
class HGMainLoop : public Loop {
public:
	static SDL_Event tEvent;
	const char* GetName() override { return "HGMainLoop"; } 
	void _RunTask() override;
	void _PaddingTask() override;
	void _StopTask() override;
};
/// \brief update loop of engine
/// \sa EngineImpl::GetUpdateLoop()
class HGUpdateLoop : public Loop {
public:
	const char* GetName() override { return "HGUpdateLoop"; }
	void _RunTask() override;
	void _PaddingTask() override;
	void _StopTask() override;
};
/// \brief render loop of engine
/// \sa EngineImpl::GetRenderLoop()
class HGRenderLoop : public Loop {
public:
	const char* GetName() override { return "HGRenderLoop"; }
	void _RunTask() override;
	void _PaddingTask() override;
	void _StopTask() override;
};

/// \brief HoneyGame������ <br>engine of HG
/// \note 
/// * ��ʼ��˳��Ӧ��Ϊ <br>the initialization order of Scenes, GameObjects, Engine Should be
/// * Scenes -> Engine -> GameObjects
class EngineImpl : public HG::IEngine {
private:
	HGMainLoop tLoopMain;
	HGUpdateLoop tLoopUpdate;
	HGRenderLoop tLoopRender;
	HGEngine::V1SDL::Thread* pUpdateThread {};
	HGEngine::V1SDL::Thread* pRenderThread {};
	HG::HGInput* pInput;
	HGEngine::V1SDL::Window* pWindow;
	
	Renderer2D* pRenderer;
	Asset* pAsset;
	AssetManager* pAssetManager;
	Scene* pCurrentScene;
	Editor* pEditor;
	HG::HGLog *pLog;
	static void SetEngine( EngineImpl* pEngine ) { EngineImpl::pEngine = pEngine; }

public:
	Physics tPhyiscs;
	/// \brief ��ȡ���߳̾�� <br>get main loop handle
	/// \note
	/// ��ѭ�����������߳�֮��
	const HGMainLoop& GetMainLoop() { return tLoopMain; }
	const HGUpdateLoop& GetUpdateLoop() { return tLoopUpdate; }
	const HGRenderLoop& GetRenderLoop() { return tLoopRender; }
	HG::HGInput* GetInput() { return pInput; }
	Scene* GetCurrentScene() { return pCurrentScene; }
	Scene* NavigateScene( const char* strSceneName );
	void NavigateScene( Scene* pScene );
	static EngineImpl* GetEngine() { return pEngine; }
	static Scene* FindScene( const char* strSceneName );
	static EngineImpl* pEngine;
	
	Editor* GetEditor() const { return pEditor; }
	Asset* GetAssetManager() const { return pAsset; }
	AssetManager* GetResourceManager() const { return pAssetManager; }
	float GetFixedUpdateTimeDelta() const { return tLoopMain.DeltaTime(); }
	HGEngine::V1SDL::Window* GetWindow() const { return pWindow; }
	Renderer2D* GetRenderer2D() const { return pRenderer; }

	void Exit();

	int Render() override;

	explicit EngineImpl( int argc, char** argv );
	virtual ~EngineImpl();
};

}
}

// Inline functions for engine access
namespace HGEngine {
namespace V1SDL {

/// \brief Get engine instance
inline EngineImpl* GetEngine()
{
    return EngineImpl::GetEngine();
}

/// \brief Get fixed update time delta
inline float GetEngineTimeDelta()
{
    return EngineImpl::GetEngine()->GetFixedUpdateTimeDelta();
}

/// \brief Find game object in current scene
inline HGEngine::V1SDL::GameObject* FindGameObject(const char* name);

/// \brief Get renderer 2D
inline Renderer2D* GetRenderer2D()
{
    return EngineImpl::GetEngine()->GetRenderer2D();
}

/// \brief Get input
inline HG::HGInput* GetInput()
{
    return EngineImpl::GetEngine()->GetInput();
}

/// \brief Get current scene
inline Scene* GetCurrentScene()
{
    return EngineImpl::GetEngine()->GetCurrentScene();
}

/// \brief Get resource manager (AssetManager)
inline AssetManager* GetResourceManager()
{
    return EngineImpl::GetEngine()->GetResourceManager();
}

} // namespace V1SDL
} // namespace HGEngine

// Legacy macros for compatibility
#define HG_ENGINE() HGEngine::V1SDL::GetEngine()
#define HG_ENGINE_TIMEDELTA HGEngine::V1SDL::GetEngineTimeDelta()
#define HG_ENGINE_FIND_GAMEOBJECT( NAME ) HGEngine::V1SDL::FindGameObject(NAME)
#define HG_ENGINE_RENDERER2D HGEngine::V1SDL::GetRenderer2D()
#define HG_ENGINE_INPUT() HGEngine::V1SDL::GetInput()
#define HG_ENGINE_CURRENT_SCENE() HGEngine::V1SDL::GetCurrentScene()
#define HG_ENGINE_ASSET() HGEngine::V1SDL::GetEngine()->GetAssetManager()
#define HG_ENGINE_ASSETMANAGER() HGEngine::V1SDL::GetAssetManager()
#define HG_ENGINE_RESOURCES() HGEngine::V1SDL::GetResourceManager()
