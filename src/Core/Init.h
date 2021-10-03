//
// Created by cyf-m on 2020/11/25.
//

#ifndef HONEYGAME_INIT_H
#define HONEYGAME_INIT_H

#include "Memory.h"

namespace __HGImpl {
    /// \brief do some init things
    /// \note
    /// this class is only a UTIL class; it does not have responsibility to release memory.
    /// the release behavior was done in ~EngineImpl::EngineImpl()
    class Init : HGCore::Memory::NonCopyable {
    public:
        Init* SDL();
        /// \brief init log
        Init* App();
    };
}

#endif //HONEYGAME_INIT_H
