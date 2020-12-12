/// \brief hg memory management
/// \file Memory.h
/// \details recover from https://github.com/cyf-gh/stLib/blob/fdc497dfaeac5de1c2aabf872478b6147e2ac936/src/core/Def.h
#pragma once

#define HG_SAFE_DEL( P ) delete P; P = nullptr
#define HG_SAFE_DEL_ARR( ARRAY ) delete []ARRAY; ARRAY = nullptr

/// \brief zero memory
/// \details len is character length, which len = 2 means 16 bits
#define st_zero_memory( p, len )memset( p, 0, len )
/// \sa st_zero_memory
#define HG_ZERO_MEMORY st_zero_memory

/// \brief ban not sure binary copy.
#define HG_MEM_NO_BINARY_COPY( CLASSNAME ) \
public: \
CLASSNAME&operator=( const CLASSNAME& rhs ) = delete; \
CLASSNAME&operator=( CLASSNAME&& rhs ) = delete;
