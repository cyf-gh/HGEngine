#pragma once 

#include <list>
#include <SDL.h>
#include <Math.hpp>
#include <Memory.h>
#include "../EngineImpl/EngineImpl.h"

namespace HG {

class HGInput : HG::Memory::NonCopyable {
public:
	enum Direct : char {
		STAY = 0,
		UP = 5,		DOWN = 9,
		LEFT = 6,	RIGHT = 8,
		UPL = 11,	UPR = 13,
		DOWNL = 15, DOWNR = 17,
	};
private:
	
	HG_INLINE bool IsUpDownDirect( const char d ) {
		return d < 10 && d % 2 == 1;
	}
	HG_INLINE bool IsRLDirect( const char d ) {
		return d < 10 && d % 2 == 0;
	}
	HG_INLINE bool IsStay( const bool d ) {
		return d == Direct::STAY;
	}
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
		case SDL_WINDOWEVENT:

		switch( pe->window.event ) {

		case SDL_WINDOWEVENT_CLOSE:   // exit game
			HG_ENGINE()->Exit();
		break;

		default:
		break;
		}
		break;
		}
	}
	/// @brief 获取当前方向键决定的方向
	/// @return HG::HGInput::Direct
	char GetDirect() {
		char d = Direct::STAY;
		for( auto tk = m_lKeyCodes.rbegin(); tk != m_lKeyCodes.rend(); ++tk ) {
			switch( *tk ) {
			case SDLK_UP:
			if( IsStay( d ) ) {
				d = Direct::UP;
				continue;
			} else {
				if( IsUpDownDirect( d ) ) {
					continue;
				} else {
					d += Direct::UP;
					goto RET;
				}
			}
			break;
			case SDLK_DOWN: 
			if( IsStay( d ) ) {
				d = Direct::DOWN;
				continue;
			} else {
				if( IsUpDownDirect( d ) ) {
					continue;
				} else {
					d += Direct::DOWN;
					goto RET;
				}
			}
			break;
			case SDLK_LEFT: 
			if( IsStay( d ) ) {
				d = Direct::LEFT;
				continue;
			} else {
				if( IsRLDirect( d ) ) {
					continue;
				} else {
					d += Direct::LEFT;
					goto RET;
				}
			}
			break;
			case SDLK_RIGHT: 
			if( IsStay( d ) ) {
				d = Direct::RIGHT;
				continue;
			} else {
				if( IsRLDirect( d ) ) {
					continue;
				} else {
					d += Direct::RIGHT;
					goto RET;
				}
			}
			break;
			default: continue; break;
			}
		}
		RET:
		return d;
	}
	HGInput() {
		tGlobalMousePos.H = 1;
		tGlobalMousePos.W = 1;
	}
};

}