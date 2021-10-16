#pragma once 
#include <Serialization.h>
#include "Transform.hpp"
#include "Animation.h"


namespace HG {
namespace Serialization {

HG_MARSHAL_FULLSPEC( __HGImpl::V1SDL::Transform ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.f64Angle );
	HG_MARSHAL_OBJECT_SETPROP( t.tLocalPos );
	HG_MARSHAL_OBJECT_SETPROP( t.tLocalRect );
	HG_MARSHAL_OBJECT_SETPROP( t.tPosition );
	HG_MARSHAL_OBJECT_SETPROP( t.tRect );
	HG_MARSHAL_OBJECT_SETPROP( t.tRotateCenter );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( __HGImpl::V1SDL::Transform ) {
	HG_UNMARSHAL_OBJECT_START;
	// Unmarshal( t.f64Angle, std::string( NAMEOF( t.f64Angle ) ).c_str(), d[std::string( NAMEOF( t.f64Angle ) ).c_str()], rd );
	assert( in.IsObject() );
	HG_UNMARSHAL_GETOBJ( t.f64Angle );
	HG_UNMARSHAL_GETOBJ( t.tLocalPos );
	HG_UNMARSHAL_GETOBJ( t.tLocalRect );
	HG_UNMARSHAL_GETOBJ( t.tPosition );
	HG_UNMARSHAL_GETOBJ( t.tRect );
	HG_UNMARSHAL_GETOBJ( t.tRotateCenter );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( __HGImpl::V1SDL::Animator2D ) {
}

}

}