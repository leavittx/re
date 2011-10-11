#pragma once

#include <SDL/SDL.h>
#include "keyboard.h"

namespace recore {

class KeyboardSDL
{
public:
	static Key resolveKeyCode(SDLKey key)
	{
		switch (key) {
		case SDLK_BACKSPACE:	return KeyBackspace;
		case SDLK_TAB:			return KeyTab;
		case SDLK_RETURN:		return KeyEnter;
		case SDLK_ESCAPE:		return KeyEsc;
		case SDLK_SPACE:		return KeySpace;
		case SDLK_LALT:			return KeyLAlt;
		case SDLK_RALT:			return KeyRAlt;
		case SDLK_LSHIFT:		return KeyLShift;
		case SDLK_RSHIFT:		return KeyRShift;
		case SDLK_LCTRL:		return KeyLCtrl;
		case SDLK_RCTRL:		return KeyRCtrl;
		case SDLK_CAPSLOCK:		return KeyCapsLock;
		case SDLK_UP:			return KeyUp;
		case SDLK_DOWN:			return KeyDown;
		case SDLK_LEFT:			return KeyLeft;
		case SDLK_RIGHT:		return KeyRight;
		case SDLK_F1:			return KeyF1;
		case SDLK_F2:			return KeyF2;
		case SDLK_F3:			return KeyF3;
		case SDLK_F4:			return KeyF4;
		case SDLK_F5:			return KeyF5;
		case SDLK_F6:			return KeyF6;
		case SDLK_F7:			return KeyF7;
		case SDLK_F8:			return KeyF8;
		case SDLK_F9:			return KeyF9;
		case SDLK_F10:			return KeyF10;
		case SDLK_F11:			return KeyF11;
		case SDLK_F12:			return KeyF12;
//		KeyF13,
//		KeyF14,
//		KeyF15,
//		KeyF16,
		case SDLK_PAGEUP:		return KeyPageUp;
		case SDLK_PAGEDOWN:		return KeyPageDown;
		case SDLK_HOME:			return KeyHome;
		case SDLK_END:			return KeyEnd;
		case SDLK_INSERT:		return KeyInsert;
		case SDLK_DELETE:		return KeyDelete;
		case SDLK_w:			return KeyW;
		case SDLK_a:			return KeyA;
		case SDLK_s:			return KeyS;
		case SDLK_d:			return KeyD;
		case SDLK_q:			return KeyQ;
		case SDLK_e:			return KeyE;
		case SDLK_1:			return Key1;
		case SDLK_2:			return Key2;
		case SDLK_3:			return Key3;
		case SDLK_4:			return Key4;
		default:				return KeyUnknown;
		}
	}
};

} // end of namespace 'recore'
