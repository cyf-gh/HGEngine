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
    public:
        Uint32 unPaddingInterval;
        Uint32 unRunInterval;
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

        Loop() : eStatus( LoopStatus::RUN ){};
        Loop(Uint32 unRunInterval, Uint32 unPaddingInterval, const LoopStatus& status )
            : unPaddingInterval(unPaddingInterval), unRunInterval( unRunInterval ), eStatus( status ) { };
        virtual ~Loop() = default;
    };

}

#endif //HONEYGAME_LOOP_H
