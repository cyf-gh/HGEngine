/// type
#pragma once

namespace HG {
    typedef int HGResult;
}

/// \brief means you can input a null pointer here, and new heap object will be allocated inside the function
#define OUT
/// \brief the argument's value changing won't be visible to outer of function
#define IN const
/// \brief means you must input a heap or stack allocated object
#define IN_OUT

/// \brief mention the function is used for callback
#define CALLBACK