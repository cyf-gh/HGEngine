#pragma once

#include <Math.hpp>
#include "Renderer2D.h"
#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {

struct DrawableGeo {
	virtual int Draw( Renderer2D* pRd ) = 0;
	virtual int Fill( Renderer2D* pRd ) = 0;
    UINT8 r, g, b, a;
    void SetColor( UINT8 r, UINT8 g, UINT8 b, UINT8 a ) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    bool IsVisiable;
    DrawableGeo() : IsVisiable( true ) {};
};

/// @brief 可绘制 Rect
class RdRect : public HG::Math::HGRect, public DrawableGeo {
private:
	SDL_Rect r;
public:
	const SDL_Rect& ToSDLRect() {
		r.x = X;
		r.y = Y;
		r.h = H;
		r.w = W;
		return r;
	}
	int Draw( Renderer2D* pRd ) override {
		ToSDLRect();
		return SDL_RenderDrawRect( pRd->pHandle, &r );
	}
	int Fill( Renderer2D* pRd) override {
		ToSDLRect();
		return SDL_RenderFillRect( pRd->pHandle, &r );
	}
    RdRect() = default;
    RdRect( const HG::Math::HGRect& rhs ) : DrawableGeo() {
        Set( rhs );
    }
    void Set( const HG::Math::HGRect& rhs ) {
        this->X = rhs.X;
        this->Y = rhs.Y;
        this->H = rhs.H;
        this->W = rhs.W;
    }
};

/// @brief 可绘制 Circle
/// @tparam _dT 
/// @ref https://gist.github.com/Gumichan01/332c26f6197a432db91cc4327fcabb1c
template<typename _dT>
class RdCircle : public HG::Math::HGCircle<_dT>, public DrawableGeo {
public:
    RdCircle() = default;
    RdCircle( const HG::Math::HGCircle<_dT>& rhs ) {
        this->tCenter = static_cast<_dT>( rhs.tCenter );
        this->Radius = static_cast<_dT>( rhs.Radius );
    }
	int Draw( Renderer2D* pRd ) override {
        _dT offsetx, offsety, d;
        int status;
        auto X = this->tCenter.X;
        auto Y = this->tCenter.Y;
        offsetx = 0;
        auto &Radius = this->Radius;
        offsety = Radius;
        d = Radius - 1;
        status = 0;

        while( offsety >= offsetx ) {
            status += SDL_RenderDrawPoint( pRd->pHandle, X + offsetx, Y + offsety );
            status += SDL_RenderDrawPoint( pRd->pHandle, X + offsety, Y + offsetx );
            status += SDL_RenderDrawPoint( pRd->pHandle, X - offsetx, Y + offsety );
            status += SDL_RenderDrawPoint( pRd->pHandle, X - offsety, Y + offsetx );
            status += SDL_RenderDrawPoint( pRd->pHandle, X + offsetx, Y - offsety );
            status += SDL_RenderDrawPoint( pRd->pHandle, X + offsety, Y - offsetx );
            status += SDL_RenderDrawPoint( pRd->pHandle, X - offsetx, Y - offsety );
            status += SDL_RenderDrawPoint( pRd->pHandle, X - offsety, Y - offsetx );

            if( status < 0 ) {
                status = -1;
                break;
            }

            if( d >= 2 * offsetx ) {
                d -= 2 * offsetx + 1;
                offsetx += 1;
            } else if( d < 2 * ( Radius - offsety ) ) {
                d += 2 * offsety - 1;
                offsety -= 1;
            } else {
                d += 2 * ( offsety - offsetx - 1 );
                offsety -= 1;
                offsetx += 1;
            }
        }
        return status;
	}
	int Fill( Renderer2D* pRd ) override {
        _dT offsetx, offsety, d;
        int status;
        auto X = this->tCenter.X;
        auto Y = this->tCenter.Y;
        offsetx = 0;
        auto& Radius = this->Radius;
        offsety = Radius;
        d = Radius - 1;
        status = 0;

        while( offsety >= offsetx ) {

            status += SDL_RenderDrawLine( pRd->pHandle, X - offsety, Y + offsetx, X + offsety, Y + offsetx );
            status += SDL_RenderDrawLine( pRd->pHandle, X - offsetx, Y + offsety, X + offsetx, Y + offsety );
            status += SDL_RenderDrawLine( pRd->pHandle, Y - offsetx, Y - offsety, X + offsetx, Y - offsety );
            status += SDL_RenderDrawLine( pRd->pHandle, Y - offsety, Y - offsetx, X + offsety, Y - offsetx );

            if( status < 0 ) {
                status = -1;
                break;
            }

            if( d >= 2 * offsetx ) {
                d -= 2 * offsetx + 1;
                offsetx += 1;
            } else if( d < 2 * ( Radius - offsety ) ) {
                d += 2 * offsety - 1;
                offsety -= 1;
            } else {
                d += 2 * ( offsety - offsetx - 1 );
                offsety -= 1;
                offsetx += 1;
            }
        }
        return status;
	}
};

}
}