//
// Created by cyf-m on 2020/12/9.
//

#ifndef HONEYGAME_GAMEOBJECT2D_H
#define HONEYGAME_GAMEOBJECT2D_H

#include "../Core/Math.h"
#include "GameObject.h"
#include "Font.hpp"
#include "../Engine/HGComponent.h"

namespace __HGImpl {
namespace V1SDL {

class Renderer2D;

class GameObject2D : public GameObject {
protected:
	SDL_Texture* m_pTexture;
	const std::string strFileName;

protected:
	void renderCameraView( Renderer2D* pRenderer );

public:
	void Update( void* pEvent ) override;
	void Render( void* pRenderer ) override;
	SDL_Texture* GetTexture() const { return m_pTexture; }
	explicit GameObject2D( const char* strObjectName = "", const char* strFileName = "" );
	virtual ~GameObject2D();
};

class GameObjectText : public GameObject2D {
private:
	SDL_Surface* m_pText;
	Font* m_pFont;
public:
	std::string Text;
	SDL_Color tColor;

	void Update( void* pEvent ) override;
	void Render( void* pRenderer ) override;
	explicit GameObjectText( const char* strObjectName, Font* pFont, const char* text = "" );
	virtual ~GameObjectText();
};
}
}

#endif //HONEYGAME_GAMEOBJECT2D_H
