//
// Created by cyf-m on 2020/11/27.
//

#ifndef HONEYGAME_ENGINEIMPL_H
#define HONEYGAME_ENGINEIMPL_H

#include "../Engine/HG.h"
#include "../Core/Loop.h"
#include "../Core/Window.h"
#include "../Core/Thread.h"
#include "Scene.h"

namespace __HGImpl { namespace V1 {
    class HGMainLoop : public HGCore::Loop {
    public:
        void _RunTask() override;
        void _PaddingTask() override;
        void _StopTask() override;
    };
    class HGUpdateLoop : public HGCore::Loop {
    public:
        void _RunTask() override;
        void _PaddingTask() override;
        void _StopTask() override;
    };
    class HGRenderLoop : public HGCore::Loop {
    public:
        void _RunTask() override;
        void _PaddingTask() override;
        void _StopTask() override;
    };
    class EngineImpl : public HG::V1::Engine {
        private:
            HGMainLoop tLoopMain;
            HGUpdateLoop tLoopUpdate;
            HGRenderLoop tLoopRender;

            HGCore::Thread *pUpdateThread{};
            HGCore::Thread *pRenderThread{};

            HGCore::Window *pWindow;

            static Scene* pCurrentScene;
        public:
            static Scene* GetCurrentScene() { return pCurrentScene; }
            int Run() override;
            EngineImpl(int argc, char **argv );
            virtual ~EngineImpl();
    };

} }

#endif //HONEYGAME_ENGINEIMPL_H
