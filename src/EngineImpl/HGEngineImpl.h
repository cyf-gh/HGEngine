//
// Created by cyf-m on 2020/11/27.
//

#ifndef HONEYGAME_HGENGINEIMPL_H
#define HONEYGAME_HGENGINEIMPL_H

#include "../Engine/HG.h"
#include "../Core/Loop.h"
#include "../Core/Window.h"
#include "../Core/Thread.h"

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
    class HGEngineImpl : public HG::V1::Engine {
        private:
            HGMainLoop tLoopMain;
            HGUpdateLoop tLoopUpdate;
            HGRenderLoop tLoopRender;

            HGCore::Thread *pUpdateThread{};
            HGCore::Thread *pRenderThread{};

            HGCore::Window *pWindow;
        public:
            int Run() override;
            HGEngineImpl( int argc, char **argv );
            virtual ~HGEngineImpl();
    };

} }

#endif //HONEYGAME_HGENGINEIMPL_H
