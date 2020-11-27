/// error code
#pragma once

#include "Type.h"

#ifdef _WIN32
#include <windows.h>
#define SYS_ERROR()  GetLastError()
#else
#include <errno.h>
#define SYS_ERROR()  errno
#endif

namespace HGCore {
    /// \brief as SDL errors have been logged by SDL log functions
    /// HG_ERR_XXX only do process that the behavior of function has done
    /// successfully or not
    enum {
        HG_ERR_OK,
        HG_ERR_NULL_OUTPUT
    };
}

#define HG_RESULT_RES HGResult res;

#define CHECK_ERR( RES_FUNC ) res = RES_FUNC; if ( res != HG_ERR_OK ) { return res; }