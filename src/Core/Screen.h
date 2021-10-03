//
// Created by cyf-m on 2020/11/25.
//

#ifndef HONEYGAME_SCREEN_H
#define HONEYGAME_SCREEN_H
#include "Error.h"
#include "Math.h"

namespace __HGImpl {
namespace V1SDL {
    class Screen {
    public:
        /// \brief get center position of screen
        HG::HGResult GetCenterPos( int w, int h, IN_OUT HG::Math::HGPos* pOutCenterPos, int displayIndex = 0);
    };
}
}

#endif //HONEYGAME_SCREEN_H
