#pragma once 

#include <list>
#include <SDL.h>
#include <Math.hpp>
#include <Memory.h>
namespace HG {

class HGInput : HG::Memory::NonCopyable {

public:
	std::list<SDL_Keycode> m_lKeyCodes;
	SDL_MouseButtonEvent* m_lMouse;
	HG::Math::HGRect tGlobalMousePos;
	HG_INLINE HG::Math::HGRect& GetGlobalMousePos( const n32 cx, const n32 cy ) {
		tGlobalMousePos.X = m_lMouse->x + cx;
		tGlobalMousePos.Y = m_lMouse->y + cy;
		return tGlobalMousePos;
	}
	void Proc( SDL_Event* pe ) {
		m_lMouse = &pe->button;
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