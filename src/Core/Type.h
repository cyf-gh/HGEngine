/// \brief HG 类型定义 <br>hg type definitions
/// \file Type.h
/// \details recover form https://github.com/cyf-gh/stLib/blob/fdc497dfaeac5de1c2aabf872478b6147e2ac936/src/core/Def.h
#pragma once

#include <cstdint>
#include <assert.h>

namespace HG {
typedef int HGResult;
struct IVoid { };
}

typedef char			ch8;
typedef int8_t			n8;
typedef int16_t		    n16;
typedef int32_t		    n32, stResult;
typedef int64_t		    n64;
typedef uint8_t		    un8, byte8;
typedef uint16_t		un16;
typedef uint32_t		un32;
typedef uint64_t		un64;
typedef float			f32;
typedef double			f64;

/// \brief means you can input a null pointer here, and new heap object will be allocated inside the function
#define OUT
/// \brief the argument's value changing won't be visible to outer of function
#define IN const
/// \brief means you must input a heap or stack allocated object
#define IN_OUT

#ifdef _MSC_VER
#   pragma warning( disable : 4996)
#   pragma warning( disable : 4114)
#   pragma warning( disable : 4180)
#   define HG_INLINE  __forceinline
		/* !_MSC_VER */
#elif defined( __GNUC__ )
#   define HG_INLINE inline
#endif  /* !__GNUC__ */

#ifdef ___HG_RELEASE___
#	define HG_ASSERT( EXP ) 
#else
#	define HG_ASSERT( EXP ) assert( EXP )
#endif





