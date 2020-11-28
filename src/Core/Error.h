/// error code
#pragma once

#include "Type.h"

#ifdef _WIN32
#include <windows.h>
#define HG_ERR_SYS_ERROR()  GetLastError()
#else
#include <errno.h>
#define HG_ERR_SYS_ERROR()  errno
#endif

namespace HGCore {
    /// \brief as SDL errors have been logged by SDL log functions
    /// HG_ERR_XXX only do process that the behavior of function has done
    /// successfully or not
    /// \sa HG_CHECK_ERR
    enum {
        HG_ERR_OK,
        HG_ERR_NULL_OUTPUT,
        HG_ERR_THREAD,
    };
}

#define HG_RESULT_RES HGResult res;
#define HG_CHECK_ERR( RES_FUNC ) res = RES_FUNC; if ( res != HG_ERR_OK ) { return res; }