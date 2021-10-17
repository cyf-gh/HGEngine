/// \brief hg math
/// \file Math.h
/// \details recover from https://github.com/cyf-gh/stLib/blob/fdc497dfaeac5de1c2aabf872478b6147e2ac936/src/utils/Maths.h
#pragma once

#include <cmath>
#include <vector> 
#include "Type.h"

namespace HG {
namespace Math {

static const f32 ST_PI = 3.141592654f;

static HG_INLINE f64 Max( f64 num1, f64 num2 ) {
	return num1 > num2 ? num1 : num2;
}

static HG_INLINE f64 Min( f64 num1, f64 num2 ) {
	return num1 > num2 ? num2 : num1;
}

template<typename _d>
static HG_INLINE f64 Abs( const _d d1 ) {
	return d1 <= 0 ? -d1 : d1;
}

static HG_INLINE bool IsEqualF_e5( f64 num1, f64 num2 ) {
	return Abs( num1 - num2 ) < 0.00001;
}

static HG_INLINE bool IsEqualF_e7( f64 num1, f64 num2 ) {
	return Abs( num1 - num2 ) < 0.000001;
}

template<typename _d>
static HG_INLINE bool IsEqual( const _d d1, const _d d2 ) {
	return d1 == d2;
}

template<>
static HG_INLINE bool IsEqual( const f64 d1, const f64 d2 ) {
	return IsEqualF_e5( d1, d2 );
}

template<>
static HG_INLINE bool IsEqual( const f32 d1, const f32 d2 ) {
	return IsEqualF_e5( d1, d2 );
}

#define IsEqualF IsEqualF_e5

static HG_INLINE bool IsIn( f64 numMin, f64 numMax, f64 numCmp ) {
	return ( ( numCmp >= numMin ) && ( numMax <= numCmp ) );
}

static HG_INLINE bool IsPositive( f64 dNum ) {
	return ( dNum > 0 );
}

static HG_INLINE f64 Avg( f64 num1, f64 num2 ) {
	return ( num1 + num2 ) / 2;
}

static HG_INLINE n32 Avg( n32 nNum1, n32 nNum2 ) {
	return ( nNum1 >> 1 ) + ( nNum2 >> 1 );
}

static HG_INLINE n32 Half( n32 nNum ) {
	return ( nNum >> 1 );
}

static HG_INLINE n32 Twofold( n32 nNum ) {
	return ( nNum << 1 );
}

static HG_INLINE n32 Opposite( n32 nNum ) {
	return ( ~nNum + 1 );
}

static HG_INLINE n32 Abs( n32 nNum ) {
	const n32 nTmp = ( nNum >> 31 );
	return ( ( nNum ^ nTmp ) - nTmp );
}

/* id Software */
static HG_INLINE f32 inv_sqrt( f32 X ) {
	f32 xhalf = 0.5f * X;
	int i = *( int* ) &X;
	i = 0x5f3759df - ( i >> 1 );
	X = *( f32* ) &i;
	X = X * ( 1.5f - xhalf * X * X );
	return X;
}


template<typename digit_type>
class HGVec2 {

public: 
static HGVec2<digit_type> kZeroVec;

public:
	digit_type X;
	digit_type Y;

	typedef digit_type trait_type;

public:
	HGVec2 UnitVec() const {
		const digit_type base = inv_sqrt( X * X + Y * Y );

		return HGVec2( X * base, Y * base );
	}
	const digit_type Norm()  const {
		const digit_type base = inv_sqrt( X * X + Y * Y );

		return ( !IsEqualF( base, 0 ) ) ?
			( 1 / base ) : 0;
	}
	
	const bool IsEqual( const HGVec2& v ) const {
		return IsEqualF( X, v.X ) && IsEqualF( Y, v.Y );
	}
	bool IsZero() const { return ( IsEqual( X, 0 ) && IsEqual( Y, 0 ) ); }

	HGVec2& Take( digit_type lmd ) {
		X *= lmd;
		Y *= lmd;
		return *this;
	}

	static HGVec2<digit_type> Center( const HGVec2& v1, const HGVec2& v2 ) {
		HGVec2<digit_type> c;
		c.X = ( v1.X + v2.X ) / 2;
		c.Y = ( v1.Y + v2.Y ) / 2;
		return c;
	}

	static bool	IsNegative( const HGVec2& v1, const HGVec2& v2 ) {
		return IsEqual( -v1.X, v2.X ) && IsEqual( -v1.Y, v2.Y );
	}

	static bool	IsParallel( const HGVec2& v1, const HGVec2& v2 ) {
		return ( v1.IsZero() || v2.IsZero() ) ?
			true : IsEqual( v1.X / v2.X, v1.Y / v2.Y );
	}

	static bool	IsVertical( const HGVec2& v1, const HGVec2& v2 ) {
		return IsEqual( GetScalarProduct( v1, v2 ), 0 );
	}

	static digit_type GetIncludeAngle( const HGVec2& v1, const HGVec2& v2 ) {
		return acos( GetScalarProduct( v1, v2 ) / ( v1.Norm() * v2.Norm() ) );
	}

	static digit_type GetScalarProduct( const HGVec2& v1, const HGVec2& v2 ) {
		return ( v1.X * v2.X ) + ( v1.Y * v2.Y );
	}

	static void Rotate( const HGVec2& vCenter, const HGVec2& v1, double a, HGVec2& vOut ) {
		digit_type x = v1.X; digit_type y = v1.Y;
		digit_type rx0 = vCenter.X; digit_type ry0 = vCenter.Y;
		vOut.X = ( x - rx0 ) * cos( a / 180 * ST_PI ) - ( y - ry0 ) * sin( a / 180 * ST_PI ) + rx0;
		vOut.Y = ( x - rx0 ) * sin( a / 180 * ST_PI ) + ( y - ry0 ) * cos( a / 180 * ST_PI ) + ry0;
	}
	static HGVec2 Add( const HGVec2& v1, const HGVec2& v2 ) {
		return HGVec2( v1.X + v2.X, v1.Y + v2.Y );
	}
	static HGVec2 Sub( const HGVec2& v1, const HGVec2& v2 ) {
		return HGVec2( v1.X - v2.X, v1.Y - v2.Y );
	}

	HGVec2( const HGVec2& v ) { X = v.X; Y = v.Y; }
	HGVec2( const digit_type X, const digit_type Y )
		: X( X ), Y( Y ) { }
	HGVec2()
		: X( 0 ), Y( 0 ) { }
	virtual	~HGVec2() { }

	HG_INLINE bool operator==( const HGVec2<digit_type> &v ) {
		return this->IsEqual( v );
	}
};

template<class digit_type> HGVec2<digit_type> HGVec2<digit_type>::kZeroVec;

struct HGPos {
	HGPos() : X( 0 ), Y( 0 ) { }
	HGPos( int X, int Y ) : X( X ), Y( Y ) { }
	n32 X, Y;
	HG_INLINE static un32 DistancePow2( n32 x1, n32 y1, n32 x2, n32 y2 ) {
		n32 d1 = x1 - x2;
		n32 d2 = y1 - y2;
		return d1 * d1 + d2 * d2;
	}
	HG_INLINE un32 DistancePow2( const HGPos& dstPos ) {
		return HGPos::DistancePow2( X, Y, dstPos.X, dstPos.Y );
	}
	template<typename digit_type>
	HG_INLINE HGVec2<digit_type> ToVec2() {
		HGVec2<digit_type> v;
		v.X = ( digit_type ) X; v.Y = ( digit_type ) Y;
		return v;
	}
};

template<typename digit_type>
struct HGSize {
	typedef digit_type trait_type;

	HGSize() : H( 0 ), W( 0 ) { }
	HGSize( int w, int h ) : H( h ), W( w ) { }
	digit_type H, W;
};

static HG_INLINE HGPos& Center( const HGVec2<float>& tPos, const HGSize<un32>& tSize, HGPos& p ) {
	p.X = static_cast< un32 >( tPos.X ) + ( tSize.W >> 1 );
	p.Y = static_cast< un32 >( tPos.Y ) + ( tSize.H >> 1 );
	return p;
}

template<typename digit_type>
struct HGCircle {
	HGVec2<digit_type> tCenter;
	digit_type Radius;
public:
	HG_INLINE bool IsOverlap( const HGCircle& c ) {
		return Radius <= HGVec2<digit_type>::Sub( tCenter, c.tCenter ).Norm();
	}
};

/// \brief 形状
/// \note  由点的有序集合组成
template<typename digit_type>
struct HGShape {
public:
	std::vector<HGVec2<digit_type>> vecPoints;
	HGShape* AddVec2( digit_type x, digit_type y ) {
		HGVec2<digit_type> v;
		v.X = x; v.Y = y;
		vecPoints.push_back( v );
		return this;
	}
	HG_INLINE HGShape* Rotate( double a, const HGVec2<digit_type>& vCenter ) {
		for( HGVec2<digit_type>& pt : vecPoints ) {
			pt.Rotate( vCenter, pt, a, pt );
		}
		return this;
	}
	/// \brief 得到外径圆
	HG_INLINE HGCircle<digit_type> GetCircumscribedCircle() {
		if( vecPoints.size() != 4 ) {
			return HGCircle<digit_type>();
		}
		HGCircle<digit_type> c;
		c.tCenter = HGVec2<digit_type>::Center( vecPoints[0], vecPoints[3] );
		c.Radius = HGVec2<digit_type>::Sub( vecPoints[0], vecPoints[3] ).Norm() / 2;
		return c;
	}
	/// \brief 获取距离原点最近的点
	HG_INLINE HGVec2<digit_type>Min() {
		digit_type min = 0;
		HGVec2<digit_type>* p = nullptr;
		for( HGVec2<digit_type>& pt : vecPoints ) {
			digit_type norm = pt.Norm();
			min = norm < min ? norm : min;
			p = &pt;
		}
		if( p == nullptr ) {
			return HGVec2<digit_type>();
		}
		return *p;
	}
	/// \brief 获取距离原点最远的点
	HG_INLINE HGVec2<digit_type>Max() {
		digit_type max = 0;
		HGVec2<digit_type>* p = nullptr;
		for( HGVec2<digit_type>& pt : vecPoints ) {
			digit_type norm = pt.Norm();
			max = norm > max ? norm : max;
			p = &pt;
		}
		if( p == nullptr ) {
			return HGVec2<digit_type>();
		}
		return *p;
	}
	HGShape() : vecPoints() { }
	/// @brief	通过[]操作符获得点坐标
	/// @param	i 点索引
	/// @return 下表索引的点
	HG_INLINE HGVec2<digit_type> operator[]( const int i ) {
		return vecPoints[i];
	}
};

struct HGRect {
	n32 X, Y;
	un32 H, W;

	HG_INLINE n32 Left() const { return X; }
	HG_INLINE n32 Right() const { return X + W; }
	HG_INLINE n32 Top() const { return Y; }
	HG_INLINE n32 Bottom() const { return Y + H; }

	template<typename digit_type>
	HG_INLINE HGShape<digit_type>& ToShape( HGShape<digit_type>& inout ) {
		inout.AddVec2( static_cast< digit_type >( Left() ), static_cast< digit_type >( Top() ) )->AddVec2( static_cast< digit_type >( Left() ), static_cast< digit_type >( Bottom() ) )
			->AddVec2( static_cast< digit_type >( Right() ), static_cast< digit_type >( Top() ) )->AddVec2( static_cast< digit_type >( Right() ), static_cast< digit_type >( Bottom() ) );
		return inout;
	}

	HG_INLINE HGPos GetCenter() const {
		HGPos p;
		p.X = this->X + ( W >> 1 );
		p.Y = this->Y + ( H >> 1 );
		return p;
	}
	/// \brief 矩形与矩形是否重叠
	/// \note  包括相交情况
	HG_INLINE bool IsOverlap( const HGRect& dstRect ) {
		return ( !( ( Right() < dstRect.Left() ) || ( Left() > dstRect.Right() ) ) &&
			!( Bottom() < dstRect.Top() || ( Top() > dstRect.Bottom() ) ) );
	}
	HG_INLINE bool IsIn( const HGRect& dstRect ) {
		const HGRect* r1 = &dstRect;
		const HGRect* r2 = this;

		return ( r1->Bottom() <= r2->Bottom() && r1->Top() >= r2->Top() && r1->Right() <= r2->Right() && r1->Left() >= r2->Left() );
	}
	HG_INLINE bool IsIntersect( const HGRect& dstRect ) {
		return ( ( Right() == dstRect.Left() ) || ( Left() == dstRect.Right() ) ||
			( Bottom() == dstRect.Top() || ( Top() == dstRect.Bottom() ) ) );
	}
	/// \brief 矩形与园是否重叠
	/// \note  包括相交情况
	template<typename digit_type>
	HG_INLINE bool IsOverlap( const HGCircle<digit_type>& dstCircle ) {
		HGVec2<digit_type> vecClosest;
		if( dstCircle.tCenter.X < X ) {
			vecClosest.X = X;
		} else if( dstCircle.tCenter.X > X + W ) {
			vecClosest.X = X + W;
		} else {
			vecClosest.X = dstCircle.tCenter.X;
		}
		if( dstCircle.tCenter.Y <= Y ) {
			vecClosest.Y = Y;
		} else if( dstCircle.tCenter.Y > Y + H ) {
			vecClosest.Y = Y + H;
		} else {
			vecClosest.Y = dstCircle.tCenter.Y;
		}
		auto centerDist = HGVec2<digit_type>::Sub( vecClosest, dstCircle.tCenter ).Norm();
		return ( centerDist < dstCircle.Radius ) || IsOverlap( HGRect {
			.X = ( n32 ) dstCircle.tCenter.X,
			.Y = ( n32 ) dstCircle.tCenter.Y,
			.H = ( un32 ) 1,
			.W = ( un32 ) 1 } );
	}

	HG_INLINE double GetDiagonal() {
		return 1 / inv_sqrt( f32( H * H + W * W ) );
	}

	template<typename digit_type>
	HG_INLINE HGCircle<digit_type> GetCircumscribedCircle() {
		HGCircle<digit_type> c;
		c.tCenter = GetCenter().ToVec2<digit_type>();
		c.Radius = GetDiagonal() / 2;
		return c;
	}
};

}
}
