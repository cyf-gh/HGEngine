/// \brief hg math
/// \file Math.h
/// \details recover from https://github.com/cyf-gh/stLib/blob/fdc497dfaeac5de1c2aabf872478b6147e2ac936/src/utils/Maths.h
#pragma once

#include <cmath>

namespace HGCore { namespace Math {

    static const f32 ST_PI = 3.141592654f;

/***********************************************************************
        Mathematical operations
***********************************************************************/

    static ST_INLINE f64 Max( f64 num1, f64 num2 ) {
        return num1 > num2 ? num1 : num2;
    }

    static ST_INLINE f64 Min( f64 num1, f64 num2 ) {
        return num1 > num2 ? num2 : num1;
    }

    static ST_INLINE bool IsEqualF( f64 num1, f64 num2 ) {
        return fabs( num1 - num2 ) < 0.000001;
    }

    static ST_INLINE bool IsIn( f64 numMin, f64 numMax, f64 numCmp ) {
        return ( ( numCmp >= numMin ) && ( numMax <= numCmp ) );
    }

    static ST_INLINE bool IsPositive( f64 dNum ) {
        return ( dNum > 0 );
    }

    static ST_INLINE f64 Avg( f64 num1, f64 num2 ) {
        return ( num1 + num2 ) / 2;
    }

    static ST_INLINE f64 Abs( f64 dNum ) {
        return dNum > 0 ? dNum : -dNum;
    }

    static ST_INLINE n32 Avg( n32 nNum1, n32 nNum2 ) {
        return ( nNum1 >> 1 ) + ( nNum2 >> 1 );
    }

    static ST_INLINE n32 Half( n32 nNum ) {
        return ( nNum >> 1 );
    }

    static ST_INLINE n32 Twofold( n32 nNum ) {
        return ( nNum << 1 );
    }

    static ST_INLINE n32 Opposite( n32 nNum ) {
        return ( ~nNum + 1 );
    }

    static ST_INLINE n32 Abs( n32 nNum ) {
        const n32 nTmp = ( nNum >> 31 );
        return ( ( nNum ^ nTmp ) - nTmp );
    }

    /* id Software */
    static ST_INLINE f32 inv_sqrt( f32 x ) {
        f32 xhalf = 0.5f * x;
        int i = * ( int * ) &x;
        i = 0x5f3759df - (i >> 1);
        x = *( f32 * ) &i;
        x = x * ( 1.5f - xhalf * x * x);
        return x;
    }

    struct HGPos {
        HGPos() : X(0), Y(0) {}
        HGPos(int x, int y) : X(x), Y(y) {}
        n32 X, Y;
        HG_INLINE static un32 DistancePow2( n32 x1, n32 y1, n32 x2, n32 y2 ) {
            n32 d1 = x1 - x2;
            n32 d2 = y1 - y2;
            return d1 * d1 + d2 * d2;
        }
        un32 DistancePow2( const HGPos& dstPos ) {
            return HGPos::DistancePow2( X, Y, dstPos.X, dstPos.Y );
        }
    };

    struct HGSize {
        HGSize() : H(0), W(0) {}
        HGSize(int w, int h) : H(h), W(w) {}
        un32 H, W;
    };

    struct HGRect {
        n32 X, Y;
        un32 H, W ;

        HG_INLINE n32 Left() const { return X; }
        HG_INLINE n32 Right() const { return X + W; }
        HG_INLINE n32 Top() const { return Y; }
        HG_INLINE n32 Bottom() const { return Y + H; }
        HG_INLINE HGPos GetCenter() const {
            HGPos p;
            p.X = this->X + ( W >> 1 );
            p.Y = this->Y + ( H >> 1 );
            return p;
        }

        HG_INLINE bool IsOverlap( const HGRect& dstRect ) {
            return !( ( Right() < dstRect.Left() ) || ( Left() > dstRect.Right() ) ||
                      ( Bottom() < dstRect.Top() || ( Top() > dstRect.Bottom() ) ) );
        }
        HG_INLINE bool IsIntersect( const HGRect& dstRect ) {
            return ( ( Right() == dstRect.Left() ) || ( Left() == dstRect.Right() ) ||
                      ( Bottom() == dstRect.Top() || ( Top() == dstRect.Bottom() ) ) );
        }

    };

} }
