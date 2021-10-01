//
// Created by cyf-m on 2020/11/27.
//

#ifndef HONEYGAME_LOOP_H
#define HONEYGAME_LOOP_H

#include <SDL.h>

namespace __HGImpl {
    /// \brief loop logic which would be used by Update Thread, Main Thread, Render Thread etc.
    /// to use it, you can inherit this class and overwrite _RunTask; _PaddingTask and _StopTask method
    class Loop {
    private:
        bool m_IsExit;
    public:
        Uint32 unPaddingInterval;
        Uint32 unRunInterval;
        Uint64 unCurrentElapsedTimeMS;
        float f32CurrentElapsedTimeS;
        double GetCurrentElpasedTimeMS() const { return static_cast<double>( f32CurrentElapsedTimeS ) * 1000; }
        double GetCurrentFps() const { return 1.0f / f32CurrentElapsedTimeS; }
        enum LoopStatus {
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
        Loop() : eStatus( LoopStatus::RUN ) {};
        Loop(Uint32 unRunInterval, Uint32 unPaddingInterval, const LoopStatus& status )
            : unPaddingInterval( unPaddingInterval ), unRunInterval( unRunInterval ), 
              eStatus( status ), m_IsExit(false), unCurrentElapsedTimeMS( 0 ) { };
        virtual ~Loop() = default;
    };

}

#endif //HONEYGAME_LOOP_H
