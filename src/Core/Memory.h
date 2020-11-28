#pragma once

#define HG_SAFE_DEL( P ) delete P; P = nullptr
#define HG_SAFE_DEL_ARR( ARRARY ) delete []ARRARY; ARRARY = nullptr

/// \brief ban not sure binary copy.
#define HG_MEM_NO_BINARY_COPY( CLASSNAME ) \
public: \
CLASSNAME&operator=( const CLASSNAME& rhs ) = delete; \
CLASSNAME&operator=( CLASSNAME&& rhs ) = delete;
