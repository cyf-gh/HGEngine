#pragma once

#include <Math.hpp>
#include "Renderer2D.h"
#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {

struct DrawableGeo {
	virtual int Draw( Renderer2D* pRd ) = 0;
	virtual int Fill( Renderer2D* pRd ) = 0;
};

class RdRect : public HG::Math::HGRect, public DrawableGeo {
public:
	SDL_Rect r;
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
};

/// @brief 
/// @tparam _dT 
/// @ref https://gist.github.com/Gumichan01/332c26f6197a432db91cc4327fcabb1c
template<typename _dT>
class RdCircle : public HG::Math::HGCircle<_dT>, public DrawableGeo {
public:
    RdCircle() = default;
    RdCircle( const HG::Math::HGCircle<_dT>& rhs ) {
        this->tCenter = rhs.tCenter;
        this->Radius = rhs.Radius;
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

class Geometry : public HG::HGComponent {
public:
    std::vector<DrawableGeo*> m_vecDrawableGeos;
	Geometry() : HG::HGComponent() { 
		nRenderIndex = HG::HGRenderableComponentSeq::GEOMETRY;
	}
	Geometry( const char* strName ) : HG::HGComponent( strName ) {
		nRenderIndex = HG::HGRenderableComponentSeq::GEOMETRY;
	}
	~Geometry() { }
private:

};

}
}