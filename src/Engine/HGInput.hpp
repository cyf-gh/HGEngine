#pragma once 

#include <list>
#include <SDL.h>
#include <Math.hpp>
#include <Memory.h>
namespace HG {

class HGInput : HG::Memory::NonCopyable {
private:
	int x, y;
public:
	std::list<SDL_Keycode> m_lKeyCodes;
	un32 m_unButtons;
	HG::Math::HGRect tGlobalMousePos;
	HG_INLINE HG::Math::HGRect& GetGlobalMousePos( const n32 cx, const n32 cy ) {
		m_unButtons = SDL_GetMouseState( &x, &y );
		tGlobalMousePos.X = x + cx;
		tGlobalMousePos.Y = y + cy;
		return tGlobalMousePos;
	}
	bool IsLeftMousePressed() {
		m_unButtons = SDL_GetMouseState( &x, &y );
		return ( m_unButtons & SDL_BUTTON_LMASK ) != 0;
	}
	void Proc( SDL_Event* pe ) {
		switch( pe->type ) {
		case SDL_KEYDOWN:
			m_lKeyCodes.push_back( pe->key.keysym.sym );
			// m_lKeyCodes.unique();
		break;
		case SDL_KEYUP:
			m_lKeyCodes.remove( pe->key.keysym.sym );
		break;
		}
	}

	HGInput() {
		tGlobalMousePos.H = 1;
		tGlobalMousePos.W = 1;
	}
};

}