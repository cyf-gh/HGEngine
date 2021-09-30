//
// Created by cyf-m on 2020/11/27.
//

#ifndef HONEYGAME_LOOP_H
#define HONEYGAME_LOOP_H

#include <SDL.h>

namespace HGCore {
    /// \brief loop logic which would be used by Update Thread, Main Thread, Render Thread etc.
    /// to use it, you can inherit this class and overwrite _RunTask; _PaddingTask and _StopTask method
    class Loop {
    private:
        bool m_IsExit;
    public:
        Uint32 unPaddingInterval;
        Uint32 unRunInterval;
        double f64CurrentElapsedTimeS;
        double GetCurrentElpasedTimeMS() const { return f64CurrentElapsedTimeS * 1000; }
        double GetCurrentFps() const { return 1 / f64CurrentElapsedTimeS; }
        enum class LoopStatus {
            RUN,
            STOP,
            PADDING
        };
        LoopStatus eStatus;
        /// \brief start loop process
        void Run();
        /// \brief method overwritten will be invoked every unRunInterval time
        /// \sa unRunInterval
        /// \sa eStatus
        virtual void _RunTask();
        /// \brief method overwritten will be invoked every unPaddingInterval time
        /// \sa unRunInterval
        /// \sa eStatus
        virtual void _PaddingTask();
        /// \brief method overwritten will be invoked when eStatus is STOP
        /// \sa eStatus
        virtual void _StopTask();

        /// \brief is loop exit
        bool IsExit() const { return m_IsExit; }

        Loop(Uint32 unRunInterval, Uint32 unPaddingInterval, const LoopStatus& status )
            : unPaddingInterval(unPaddingInterval), unRunInterval( unRunInterval ), eStatus( status ), f64CurrentElapsedTimeS( 0 ), m_IsExit(false) { };
        virtual ~Loop() = default;
    };

}

#endif //HONEYGAME_LOOP_H
