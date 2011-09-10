#pragma once

#include <SDL/SDL.h>
#include "keyboard.h"

namespace recore {

class KeyboardSDL
{
public:
	static Key resolveKeyCode(SDLKey key)
	{
		switch (key)
		{
		case SDLK_BACKSPACE:	return KeyBackspace;
		case SDLK_TAB:			return KeyTab;
		case SDLK_RETURN:		return KeyEnter;
		case SDLK_ESCAPE:		return KeyEsc;
		case SDLK_SPACE:		return KeySpace;
//		KeyAlt,
//		KeyShift,
//		KeyCtrl,
//		KeyCapsLock,
//		KeyUpArrow,
//		KeyDownArrow,
//		KeyLeftArrow,
//		KeyRightArrow,
		case SDLK_F1:			return KeyF1;
		case SDLK_F2:			return KeyF2;
		case SDLK_F3:			return KeyF3;
		case SDLK_F4:			return KeyF4;
		case SDLK_F5:			return KeyF5;
		case SDLK_F6:			return KeyF6;
		case SDLK_F7:			return KeyF7;
		case SDLK_F8:			return KeyF8;
//		KeyF9,
//		KeyF10,
//		KeyF11,
//		KeyF12,
//		KeyF13,
//		KeyF14,
//		KeyF15,
//		KeyF16,
//		KeyPageUp,
//		KeyPageDown,
//		KeyHome,
//		KeyEnd,
//		KeyInsert,
//		KeyDelete,
		case SDLK_s:			return KeyS;
		default:				return KeyUnknown;
		}
	}
};

} // end of namespace 'recore'
