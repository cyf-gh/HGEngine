//
// Created by cyf-m on 2020/11/25.
//

#ifndef HONEYGAME_INIT_H
#define HONEYGAME_INIT_H

#include "Memory.h"

namespace HGCore {
    class Init {
        HG_MEM_NO_BINARY_COPY( Init );
    public:
        void SDL();
        void App();
    };
}

#endif //HONEYGAME_INIT_H
