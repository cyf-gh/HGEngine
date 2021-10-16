#pragma once 
#include <Serialization.h>
#include "Transform.hpp"
#include "Animation.h"

namespace HG {
namespace Serialization {

HG_MARSHAL_FULL_SPEC( __HGImpl::V1SDL::Transform ) {
	HG_BOOST_PTREEOUT( t.f64Angle );
	HG_BOOST_PTREEOUT_START;
	HG_BOOST_PTREEOUT_CLASS( t.tLocalPos );
	HG_BOOST_PTREEOUT_CLASS( t.tLocalRect );
	HG_BOOST_PTREEOUT_CLASS( t.tPosition );
	HG_BOOST_PTREEOUT_CLASS( t.tRect );
	HG_BOOST_PTREEOUT_CLASS( t.tRotateCenter );
	return out;
}

HG_MARSHAL_FULL_SPEC( __HGImpl::V1SDL::Animator2D ) {

}

}

}