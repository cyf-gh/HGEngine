//
// Created by cyf-m on 2020/12/9.
//

#ifndef HONEYGAME_GAMEOBJECT2D_H
#define HONEYGAME_GAMEOBJECT2D_H

#include <Math.hpp>
#include "GameObject.h"
#include "Font.hpp"
#include "../Engine/HGComponent.h"

namespace HGEngine {
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
	/// \brief 克隆2D游戏对象
	/// \note 
	///	* 克隆后的2D游戏对象公用同一 Texture
	GameObject* Clone() override { return new GameObject2D(*this); }
	SDL_Texture* GetTexture() const { return m_pTexture; }
	HG::Math::HGSize<int> GetTextureSize();
	void SetTexture( SDL_Texture* pT ) { m_pTexture = pT; }
	explicit GameObject2D( const char* strObjectName = "", const char* strFileName = "" );
	virtual ~GameObject2D();
};

class GameObjectText : public GameObject2D {
private:
	SDL_Surface* m_pText;
	FontImpl* m_pFont;
public:
	std::string Text;
	SDL_Color tColor;

	void Update( void* pEvent ) override;
	void Render( void* pRenderer ) override;
	explicit GameObjectText( const char* strObjectName, FontImpl* pFont, const char* text = "" );
	virtual ~GameObjectText();
};
}
}

#endif //HONEYGAME_GAMEOBJECT2D_H
