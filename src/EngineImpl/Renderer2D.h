//
// Created by cyf-m on 2020/12/10.
//

#ifndef HONEYGAME_RENDERER2D_H
#define HONEYGAME_RENDERER2D_H

#include <unordered_map>
#include "Renderer.h"

namespace HGEngine {
namespace V1SDL {

class DrawableGeo;
class GameObject;

class Renderer2D : public HGEngine::V1SDL::Renderer {
public:
	/// \brief create a texture via file name, after you have
	SDL_Texture* CreateTextureFromFile( const char* pStrFileName );
	void Copy( const GameObject* pGameObject, const SDL_Rect* pSrcRect,const SDL_Rect* pDstRect );
	void CopyEx( const GameObject* pGameObject, const SDL_Rect* pSrcRect,const SDL_Rect* pDstRect, const double f64Angle, const SDL_Point* pCenter, const SDL_RendererFlip &tFlip );
	void SetDrawColor( Uint8 r, Uint8 g, Uint8 b, Uint8 a );
	void SetDrawColor( const DrawableGeo* pdg );
	void SetDrawColor( const HG::Math::HGColor& tc );
	void SetDrawBlendMode( const SDL_BlendMode eBm = SDL_BLENDMODE_BLEND );
	int DrawLine( int x, int y, int xx, int yy );
	explicit Renderer2D(
		SDL_Window* pWindow,
		int index = -1,
		Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC,
		const SDL_BlendMode eBm = SDL_BLENDMODE_BLEND )
		: Renderer( pWindow, index, flags ) { 
		SetDrawBlendMode( eBm );
	};
};
}
}

#endif //HONEYGAME_RENDERER2D_H
