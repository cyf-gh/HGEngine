/// \brief hg memory management
/// \file Memory.h
/// \details recover from https://github.com/cyf-gh/stLib/blob/fdc497dfaeac5de1c2aabf872478b6147e2ac936/src/core/Def.h
#pragma once

#include <string>

#define HG_SAFE_DEL( P ) if ( P != nullptr ) { delete P; P = nullptr; }
#define HG_SAFE_DEL_ARR( ARRAY ) if ( ARRAY != nullptr ) { delete []ARRAY; ARRAY = nullptr; }

/// \brief zero memory
/// \details len is character length, which len = 2 means 16 bits
#define st_zero_memory( p, len )memset( p, 0, len )
/// \sa st_zero_memory
#define HG_ZERO_MEMORY st_zero_memory

namespace HG {
namespace Memory {
	/// \brief 对象拷贝 <br>object non copyable
	class NonCopyable {
	protected:
		NonCopyable() = default;
		virtual ~NonCopyable() = default;

	public:
		NonCopyable(const NonCopyable &) = delete;
		NonCopyable(NonCopyable &&) noexcept = default;
		NonCopyable &operator=(const NonCopyable &) = delete;
		NonCopyable &operator=(NonCopyable &&) noexcept = default;
	};
}
}

#define HG_EMPTY_STRING ""
#define HG_EMPTY_STRINGW _T("")

/// \brief ban not sure binary copy.\
/// \note 
/// * 推荐使用HG::Memory::NonCopyable <br>HG::Memory::NonCopyable is recommended
#define HG_MEM_NO_BINARY_COPY( CLASSNAME ) \
public: \
CLASSNAME&operator=( const CLASSNAME& rhs ) = delete; \
CLASSNAME&operator=( CLASSNAME&& rhs ) = delete;
