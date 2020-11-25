/// error code
#pragma once

#include "Type.h"

namespace HG {
    /// \brief as SDL errors have been logged by SDL log functions
    /// HG_ERR_XXX only do process that the behavior of function has done
    /// successfully or not
    enum {
        HG_ERR_OK,
        HG_ERR_NULL_OUTPUT
    };
}