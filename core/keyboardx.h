#pragma once

#include <X11/keysym.h>
#include "keyboard.h"

namespace recore {

class KeyboardX
{
public:
	static Key resolveKeyCode(unsigned int key)
	{
		switch (key) {
		case XK_Tab:                    return KeyTab;
		case XK_Return:                 return KeyEnter;
		case XK_space:					return KeySpace;
//		case XK_Menu:                   return KeyAlt;
		case XK_Shift_L:				return KeyLShift;
		case XK_Shift_R:                return KeyRShift;
		case XK_Control_L:				return KeyLCtrl;
		case XK_Control_R:              return KeyRCtrl;
		case XK_Caps_Lock:              return KeyCapsLock;
		case XK_Up:                     return KeyUp;
		case XK_Down:                   return KeyDown;
		case XK_Left:                   return KeyLeft;
		case XK_Right:                  return KeyRight;
		case XK_BackSpace:              return KeyBackspace;
		case XK_F1:                     return KeyF1;
		case XK_F2:                     return KeyF2;
		case XK_F3:                     return KeyF3;
		case XK_F4:                     return KeyF4;
		case XK_F5:                     return KeyF5;
		case XK_F6:                     return KeyF6;
		case XK_F7:                     return KeyF7;
		case XK_F8:                     return KeyF8;
		case XK_F9:                     return KeyF9;
		case XK_F10:                    return KeyF10;
		case XK_F11:                    return KeyF11;
		case XK_F12:                    return KeyF12;
		case XK_F13:                    return KeyF13;
		case XK_F14:                    return KeyF14;
		case XK_F15:                    return KeyF15;
		case XK_F16:                    return KeyF16;
		case XK_Prior:                  return KeyPageUp;
		case XK_Next:                   return KeyPageDown;
		case XK_Home:                   return KeyHome;
		case XK_End:                    return KeyEnd;
		case XK_Insert:                 return KeyInsert;
		case XK_Delete:                 return KeyDelete;
		case XK_Escape:                 return KeyEsc;
//		case 0xFE03:                    return 0; // ALT GR
		case XK_w:
		case XK_W:						return KeyW;
		case XK_a:
		case XK_A:						return KeyA;
		case XK_s:
		case XK_S:                      return KeyS;
		case XK_d:
		case XK_D:						return KeyD;
		case XK_q:
		case XK_Q:                      return KeyQ;
		case XK_e:
		case XK_E:						return KeyE;
		case XK_1:						return Key1;
		case XK_2:						return Key2;
		case XK_3:						return Key3;
		case XK_4:						return Key4;
		default:						return KeyUnknown;
		}
	}
};

} // end of namespace 'recore'
