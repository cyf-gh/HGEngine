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

/// \brief 形状，包含位置与大小信息 （figure, which contains position and size infomation）
class Figure : public HG::V1SDL::HGComponent {
public:
	__HGImpl::Math::HGPos tPosition;
	__HGImpl::Math::HGSize tRect;
	SDL_Rect ToSDLRect() {
		return SDL_Rect {
			.x = tPosition.X,
			.y = tPosition.Y,
			.w = static_cast< int >( tRect.W ),
			.h = static_cast< int >( tRect.H ),
		};
	}
	void Zero() {
		tPosition.X = 0;
		tPosition.Y = 0;
		tRect.H = 0;
		tRect.W = 0;
	}
	Figure( const char* strName ) : HG::V1SDL::HGComponent( strName ) {
		Zero();
	}
	bool IsZero() const {
		return
			tPosition.X == 0 &&
			tPosition.Y == 0 &&
			tRect.H == 0 &&
			tRect.W == 0;
	}
};

class GameObject2D : public GameObject {
protected:
	SDL_Texture* m_pTexture;
	const std::string strFileName;

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
