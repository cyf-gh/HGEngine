#include <Math.hpp>
#include <utest.h>

using namespace HG::Math;

UTEST( Math, HGRect ) {
	HG::Math::HGRect r {
		.X = 0,
		.Y = 0,
		.H = 600,
		.W = 800
	};
	auto c = r.GetCenter();
	ASSERT_TRUE( c.X == 400 && c.Y == 300 );
	ASSERT_TRUE( r.Left() == 0 );
	ASSERT_TRUE( r.Right() == 800 );
	ASSERT_TRUE( r.Top() == 0 );
	ASSERT_TRUE( r.Bottom() == 600 );
}

UTEST( Math, HGShape ) {
	HG::Math::HGRect rect {
		.X = 0,
		.Y = 0,
		.H = 300,
		.W = 300
	};
	auto center = rect.GetCenter();
	HG::Math::HGShape<float> tRectShape;
	rect.ToShape<float>( tRectShape );
	auto first = tRectShape.vecPoints[0];
	tRectShape.Rotate( 90, rect.GetCenter().ToVec2<float>() );
	ASSERT_TRUE( first.IsEqual( tRectShape[1] ) );
}

UTEST( Math, HGVec2 ) {
	HGVec2<float> a;
	ASSERT_TRUE( a == HGVec2<float>::kZeroVec );
}
