#pragma once

#include <Math.hpp>
#include "Renderer2D.h"
#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {

struct RenderableGeo {
	virtual int Draw( Renderer2D* pRd ) = 0;
	virtual int Fill( Renderer2D* pRd ) = 0;
};

class RdRect : public HG::Math::HGRect, public RenderableGeo {
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
class RdCircle : public HG::Math::HGCircle<_dT>, public RenderableGeo {
public:
	int Draw( Renderer2D* pRd ) override {
        int offsetx, offsety, d;
        int status;

        offsetx = 0;
        offsety = radius;
        d = Radius - 1;
        status = 0;

        while( offsety >= offsetx ) {
            status += SDL_RenderDrawPoint( renderer, X + offsetx, Y + offsety );
            status += SDL_RenderDrawPoint( renderer, X + offsety, Y + offsetx );
            status += SDL_RenderDrawPoint( renderer, X - offsetx, Y + offsety );
            status += SDL_RenderDrawPoint( renderer, X - offsety, Y + offsetx );
            status += SDL_RenderDrawPoint( renderer, X + offsetx, Y - offsety );
            status += SDL_RenderDrawPoint( renderer, X + offsety, Y - offsetx );
            status += SDL_RenderDrawPoint( renderer, X - offsetx, Y - offsety );
            status += SDL_RenderDrawPoint( renderer, X - offsety, Y - offsetx );

            if( status < 0 ) {
                status = -1;
                break;
            }

            if( d >= 2 * offsetx ) {
                d -= 2 * offsetx + 1;
                offsetx += 1;
            } else if( d < 2 * ( radius - offsety ) ) {
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
        int offsetx, offsety, d;
        int status;

        offsetx = 0;
        offsety = Radius;
        d = Radius - 1;
        status = 0;

        while( offsety >= offsetx ) {

            status += SDL_RenderDrawLine( renderer, X - offsety, Y + offsetx, X + offsety, Y + offsetx );
            status += SDL_RenderDrawLine( renderer, X - offsetx, Y + offsety, X + offsetx, Y + offsety );
            status += SDL_RenderDrawLine( renderer, Y - offsetx, Y - offsety, X + offsetx, Y - offsety );
            status += SDL_RenderDrawLine( renderer, Y - offsety, Y - offsetx, X + offsety, Y - offsetx );

            if( status < 0 ) {
                status = -1;
                break;
            }

            if( d >= 2 * offsetx ) {
                d -= 2 * offsetx + 1;
                offsetx += 1;
            } else if( d < 2 * ( radius - offsety ) ) {
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
	HG_COMPONENT_RENDERABLE

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