/// \file EngineImpl.h
/// \brief Engine implementation

#ifndef HONEYGAME_ENGINEIMPL_H
#define HONEYGAME_ENGINEIMPL_H

#include "../Engine/HG.h"
#include "../Core/Loop.h"
#include "../Core/Window.h"
#include "../Core/Thread.h"
#include "Renderer2D.h"

namespace __HGImpl { namespace V1SDL {
    class Scene;
    /// \brief main loop of engine
    /// \details main loop is a event loop main thread
    /// , faster is better
    /// \sa EngineImpl::GetMainLoop()
    class HGMainLoop : public __HGImpl::Loop {
    public:
        void _RunTask() override;
        void _PaddingTask() override;
        void _StopTask() override;
    };
    /// \brief update loop of engine
    /// \sa EngineImpl::GetUpdateLoop()
    class HGUpdateLoop : public __HGImpl::Loop {
    public:
        void _RunTask() override;
        void _PaddingTask() override;
        void _StopTask() override;
    };
    /// \brief render loop of engine
    /// \sa EngineImpl::GetRenderLoop()
    class HGRenderLoop : public __HGImpl::Loop {
    public:
        void _RunTask() override;
        void _PaddingTask() override;
        void _StopTask() override;
    };

    /// \brief the engine of HG
    /// \note the initialization order of Scenes, GameObjects, Engine Should be
    /// Scenes -> Engine -> GameObjects
    class EngineImpl : public HG::V1SDL::IEngine {
        private:
            HGMainLoop tLoopMain;
            HGUpdateLoop tLoopUpdate;
            HGRenderLoop tLoopRender;

            __HGImpl::Thread *pUpdateThread{};
            __HGImpl::Thread *pRenderThread{};

            __HGImpl::Window *pWindow;

            Renderer2D *pRenderer;

            Scene* pCurrentScene;
            static void SetEngine( EngineImpl* pEngine ) { EngineImpl::pEngine = pEngine; }

        public:
            const HGMainLoop& GetMainLoop() { return tLoopMain; }
            const HGUpdateLoop& GetUpdateLoop() { return tLoopUpdate; }
            const HGRenderLoop& GetRenderLoop() { return tLoopRender; }

            Scene* GetCurrentScene() { return pCurrentScene; }
            void NavigateScene(const char * strSceneName );
            static EngineImpl* GetEngine() { return pEngine; }
            static EngineImpl* pEngine;

            __HGImpl::Window * GetWindow() const { return pWindow; }
            Renderer2D * GetRenderer2D() const { return pRenderer; }

            void Exit();

            int Run() override;

            explicit EngineImpl(int argc, char **argv );
            virtual ~EngineImpl();
    };

} }

#endif //HONEYGAME_ENGINEIMPL_H
