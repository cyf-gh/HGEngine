//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_HG_H
#define HONEYGAME_HG_H

#define HG_ENGINE_THREAD_READ_WRITE

namespace HG { namespace V1 {
    class Engine {
        /// \brief this is main thread of game; this is a Read-Write thread
        /// \note process for user input.
        virtual int HG_ENGINE_THREAD_READ_WRITE Run() = 0;
    };
} }


#endif //HONEYGAME_HG_H
