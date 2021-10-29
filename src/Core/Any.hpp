#pragma once
#include <typeinfo>
#include "Type.h"
#include "Memory.h"

namespace HG {

template<typename T = void>
class Any {
public:
	type_info m_tTypeInfo;
	template<typename _T>
	Any( const _T& tt ) : pH( new _T( tt ) ), m_tTypeInfo( typeid( _T ) ) { 
		
	}
	Any( const Any& tt ) : pH( new Any( tt ) ), m_tTypeInfo( tt.m_tTypeInfo ) { }

	template<typename _T>
	Any& operator=( const _T& tt ) {
		pH = new _T( tt );
		m_tTypeInfo = typeid( _T );
		return this;
	}
	~Any(){
		HG_SAFE_DEL( pH );
	}

private:
	void *pH;
};

}