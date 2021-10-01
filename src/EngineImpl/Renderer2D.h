//
// Created by cyf-m on 2020/12/10.
//

#ifndef HONEYGAME_RENDERER2D_H
#define HONEYGAME_RENDERER2D_H

#include "../Core/Renderer.h"
#include "GameObject2D.h"
#include <unordered_map>

namespace __HGImpl {
namespace V1SDL {
class Renderer2D : public __HGImpl::Renderer {
public:
	/// \brief create a texture via file name, after you have
	SDL_Texture* CreateTextureFromFile( const char* pStrFileName );
	std::unordered_map<std::string, SDL_Texture*> m_umTextures;
	void Copy( const GameObject2D* pGameObject, const SDL_Rect* pSrcRect,const SDL_Rect* pDstRect );
	Renderer2D(
		SDL_Window* pWindow,
		int index = -1,
		Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC )
		: Renderer( pWindow, index, flags ), m_umTextures( std::unordered_map<std::string, SDL_Texture*>() ) { };
};
}
}

#endif //HONEYGAME_RENDERER2D_H
