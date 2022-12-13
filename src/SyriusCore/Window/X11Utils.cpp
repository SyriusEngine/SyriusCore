#include "X11Utils.hpp"

#if defined(SR_CORE_PLATFORM_LINUX)

namespace Syrius{

    SR_KEYBOARD_KEY convertVirtualKey(KeySym sym){
        switch (sym) {
            case XK_Shift_L:      return SR_KEY_LEFT_SHIFT;
            case XK_Shift_R:      return SR_KEY_RIGHT_SHIFT;
            case XK_Control_L:    return SR_KEY_LEFT_CONTROL;
            case XK_Control_R:    return SR_KEY_RIGHT_CONTROL;
            case XK_Alt_L:        return SR_KEY_LEFT_ALT;
            case XK_Alt_R:        return SR_KEY_RIGHT_ALT;
            case XK_Super_L:      return SR_KEY_LEFT_SUPER;
            case XK_Super_R:      return SR_KEY_RIGHT_SUPER;
            case XK_Menu:         return SR_KEY_MENU;
            case XK_Escape:       return SR_KEY_ESCAPE;
            case XK_semicolon:    return SR_KEY_SEMICOLON;
            case XK_slash:        return SR_KEY_SLASH;
            case XK_equal:        return SR_KEY_EQUAL;
            case XK_minus:        return SR_KEY_MINUS;
            case XK_bracketleft:  return SR_KEY_LEFT_BRACKET;
            case XK_bracketright: return SR_KEY_RIGHT_BRACKET;
            case XK_comma:        return SR_KEY_COMMA;
            case XK_period:       return SR_KEY_PERIOD;
            case XK_apostrophe:   return SR_KEY_APOSTROPHE;
            case XK_backslash:    return SR_KEY_BACKSLASH;
            case XK_grave:        return SR_KEY_GRAVE_ACCENT;
            case XK_space:        return SR_KEY_SPACE;
            case XK_Return:       return SR_KEY_ENTER;
            case XK_KP_Enter:     return SR_KEY_KP_ENTER;
            case XK_BackSpace:    return SR_KEY_BACKSPACE;
            case XK_Tab:          return SR_KEY_TAB;
            case XK_Prior:        return SR_KEY_PAGE_UP;
            case XK_Next:         return SR_KEY_PAGE_DOWN;
            case XK_End:          return SR_KEY_END;
            case XK_Home:         return SR_KEY_HOME;
            case XK_Insert:       return SR_KEY_INSERT;
            case XK_Delete:       return SR_KEY_DELETE;
            case XK_KP_Add:       return SR_KEY_KP_ADD;
            case XK_KP_Subtract:  return SR_KEY_KP_SUBTRACT;
            case XK_KP_Multiply:  return SR_KEY_KP_MULTIPLY;
            case XK_KP_Divide:    return SR_KEY_KP_DIVIDE;
            case XK_Pause:        return SR_KEY_PAUSE;
            case XK_F1:             return SR_KEY_F1;
            case XK_F2:             return SR_KEY_F2;
            case XK_F3:             return SR_KEY_F3;
            case XK_F4:             return SR_KEY_F4;
            case XK_F5:             return SR_KEY_F5;
            case XK_F6:             return SR_KEY_F6;
            case XK_F7:             return SR_KEY_F7;
            case XK_F8:             return SR_KEY_F8;
            case XK_F9:             return SR_KEY_F9;
            case XK_F10:            return SR_KEY_F10;
            case XK_F11:            return SR_KEY_F11;
            case XK_F12:            return SR_KEY_F12;
            case XK_F13:            return SR_KEY_F13;
            case XK_F14:            return SR_KEY_F14;
            case XK_F15:            return SR_KEY_F15;
            case XK_F16:            return SR_KEY_F16;
            case XK_F17:            return SR_KEY_F17;
            case XK_F18:            return SR_KEY_F18;
            case XK_F19:            return SR_KEY_F19;
            case XK_F20:            return SR_KEY_F20;
            case XK_F21:            return SR_KEY_F21;
            case XK_F22:            return SR_KEY_F22;
            case XK_F23:            return SR_KEY_F23;
            case XK_F24:            return SR_KEY_F24;
            case XK_Left:           return SR_KEY_LEFT;
            case XK_Right:          return SR_KEY_RIGHT;
            case XK_Up:             return SR_KEY_UP;
            case XK_Down:           return SR_KEY_DOWN;
            case XK_0:              return SR_KEY_0;
            case XK_1:              return SR_KEY_1;
            case XK_2:              return SR_KEY_2;
            case XK_3:              return SR_KEY_3;
            case XK_4:              return SR_KEY_4;
            case XK_5:              return SR_KEY_5;
            case XK_6:              return SR_KEY_6;
            case XK_7:              return SR_KEY_7;
            case XK_8:              return SR_KEY_8;
            case XK_9:              return SR_KEY_9;
            case XK_Q:              return SR_KEY_Q;
            case XK_W:              return SR_KEY_W;
            case XK_E:              return SR_KEY_E;
            case XK_R:              return SR_KEY_R;
            case XK_T:              return SR_KEY_T;
            case XK_Y:              return SR_KEY_Y;
            case XK_U:              return SR_KEY_U;
            case XK_I:              return SR_KEY_I;
            case XK_O:              return SR_KEY_O;
            case XK_P:              return SR_KEY_P;
            case XK_A:              return SR_KEY_A;
            case XK_S:              return SR_KEY_S;
            case XK_D:              return SR_KEY_D;
            case XK_F:              return SR_KEY_F;
            case XK_G:              return SR_KEY_G;
            case XK_H:              return SR_KEY_H;
            case XK_J:              return SR_KEY_J;
            case XK_K:              return SR_KEY_K;
            case XK_L:              return SR_KEY_L;
            case XK_Z:              return SR_KEY_Z;
            case XK_X:              return SR_KEY_X;
            case XK_C:              return SR_KEY_C;
            case XK_V:              return SR_KEY_V;
            case XK_B:              return SR_KEY_B;
            case XK_N:              return SR_KEY_N;
            case XK_M:              return SR_KEY_M;
            case XK_KP_Insert:      return SR_KEY_NUMPAD_0;
            case XK_KP_End:         return SR_KEY_NUMPAD_1;
            case XK_KP_Down:        return SR_KEY_NUMPAD_2;
            case XK_KP_Page_Down:   return SR_KEY_NUMPAD_3;
            case XK_KP_Left:        return SR_KEY_NUMPAD_4;
            case XK_KP_Begin:       return SR_KEY_NUMPAD_5;
            case XK_KP_Right:       return SR_KEY_NUMPAD_6;
            case XK_KP_Home:        return SR_KEY_NUMPAD_7;
            case XK_KP_Up:          return SR_KEY_NUMPAD_8;
            case XK_KP_Page_Up:     return SR_KEY_NUMPAD_9;
            default:                return SR_KEY_NONE;
        }
    }

}

#endif

