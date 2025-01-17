#include "Glfw3Utils.hpp"

#if !defined(SR_PLATFORM_WIN64)

namespace Syrius {

    SR_KEYBOARD_KEY translateGlfwKey(const int key) {
        switch (key) {
            case GLFW_KEY_SPACE:        return SR_KEY_SPACE;
            case GLFW_KEY_APOSTROPHE:   return SR_KEY_APOSTROPHE;
            case GLFW_KEY_COMMA:        return SR_KEY_COMMA;
            case GLFW_KEY_MINUS:        return SR_KEY_MINUS;
            case GLFW_KEY_PERIOD:       return SR_KEY_PERIOD;
            case GLFW_KEY_SLASH:        return SR_KEY_SLASH;
            case GLFW_KEY_0:            return SR_KEY_0;
            case GLFW_KEY_1:            return SR_KEY_1;
            case GLFW_KEY_2:            return SR_KEY_2;
            case GLFW_KEY_3:            return SR_KEY_3;
            case GLFW_KEY_4:            return SR_KEY_4;
            case GLFW_KEY_5:            return SR_KEY_5;
            case GLFW_KEY_6:            return SR_KEY_6;
            case GLFW_KEY_7:            return SR_KEY_7;
            case GLFW_KEY_8:            return SR_KEY_8;
            case GLFW_KEY_9:            return SR_KEY_9;
            case GLFW_KEY_SEMICOLON:    return SR_KEY_SEMICOLON;
            case GLFW_KEY_EQUAL:        return SR_KEY_EQUAL;
            case GLFW_KEY_A:            return SR_KEY_A;
            case GLFW_KEY_B:            return SR_KEY_B;
            case GLFW_KEY_C:            return SR_KEY_C;
            case GLFW_KEY_D:            return SR_KEY_D;
            case GLFW_KEY_E:            return SR_KEY_E;
            case GLFW_KEY_F:            return SR_KEY_F;
            case GLFW_KEY_G:            return SR_KEY_G;
            case GLFW_KEY_H:            return SR_KEY_H;
            case GLFW_KEY_I:            return SR_KEY_I;
            case GLFW_KEY_J:            return SR_KEY_J;
            case GLFW_KEY_K:            return SR_KEY_K;
            case GLFW_KEY_L:            return SR_KEY_L;
            case GLFW_KEY_M:            return SR_KEY_M;
            case GLFW_KEY_N:            return SR_KEY_N;
            case GLFW_KEY_O:            return SR_KEY_O;
            case GLFW_KEY_P:            return SR_KEY_P;
            case GLFW_KEY_Q:            return SR_KEY_Q;
            case GLFW_KEY_R:            return SR_KEY_R;
            case GLFW_KEY_S:            return SR_KEY_S;
            case GLFW_KEY_T:            return SR_KEY_T;
            case GLFW_KEY_U:            return SR_KEY_U;
            case GLFW_KEY_V:            return SR_KEY_V;
            case GLFW_KEY_W:            return SR_KEY_W;
            case GLFW_KEY_X:            return SR_KEY_X;
            case GLFW_KEY_Y:            return SR_KEY_Y;
            case GLFW_KEY_Z:            return SR_KEY_Z;
            case GLFW_KEY_LEFT_BRACKET: return SR_KEY_LEFT_BRACKET;
            case GLFW_KEY_BACKSLASH:    return SR_KEY_BACKSLASH;
            case GLFW_KEY_RIGHT_BRACKET:return SR_KEY_RIGHT_BRACKET;
            case GLFW_KEY_GRAVE_ACCENT: return SR_KEY_GRAVE_ACCENT;
            case GLFW_KEY_WORLD_1:      return SR_KEY_WORLD_1;
            case GLFW_KEY_WORLD_2:      return SR_KEY_WORLD_2;
            case GLFW_KEY_ESCAPE:       return SR_KEY_ESCAPE;
            case GLFW_KEY_ENTER:        return SR_KEY_ENTER;
            case GLFW_KEY_TAB:          return SR_KEY_TAB;
            case GLFW_KEY_BACKSPACE:    return SR_KEY_BACKSPACE;
            case GLFW_KEY_INSERT:       return SR_KEY_INSERT;
            case GLFW_KEY_DELETE:       return SR_KEY_DELETE;
            case GLFW_KEY_RIGHT:        return SR_KEY_RIGHT;
            case GLFW_KEY_LEFT:         return SR_KEY_LEFT;
            case GLFW_KEY_DOWN:         return SR_KEY_DOWN;
            case GLFW_KEY_UP:           return SR_KEY_UP;
            case GLFW_KEY_PAGE_UP:      return SR_KEY_PAGE_UP;
            case GLFW_KEY_PAGE_DOWN:    return SR_KEY_PAGE_DOWN;
            case GLFW_KEY_HOME:         return SR_KEY_HOME;
            case GLFW_KEY_END:          return SR_KEY_END;
            case GLFW_KEY_CAPS_LOCK:    return SR_KEY_CAPS_LOCK;
            case GLFW_KEY_SCROLL_LOCK:  return SR_KEY_SCROLL_LOCK;
            case GLFW_KEY_NUM_LOCK:     return SR_KEY_NUM_LOCK;
            case GLFW_KEY_PRINT_SCREEN: return SR_KEY_PRINT_SCREEN;
            case GLFW_KEY_PAUSE:        return SR_KEY_PAUSE;
            case GLFW_KEY_F1:           return SR_KEY_F1;
            case GLFW_KEY_F2:           return SR_KEY_F2;
            case GLFW_KEY_F3:           return SR_KEY_F3;
            case GLFW_KEY_F4:           return SR_KEY_F4;
            case GLFW_KEY_F5:           return SR_KEY_F5;
            case GLFW_KEY_F6:           return SR_KEY_F6;
            case GLFW_KEY_F7:           return SR_KEY_F7;
            case GLFW_KEY_F8:           return SR_KEY_F8;
            case GLFW_KEY_F9:           return SR_KEY_F9;
            case GLFW_KEY_F10:          return SR_KEY_F10;
            case GLFW_KEY_F11:          return SR_KEY_F11;
            case GLFW_KEY_F12:          return SR_KEY_F12;
            case GLFW_KEY_F13:          return SR_KEY_F13;
            case GLFW_KEY_F14:          return SR_KEY_F14;
            case GLFW_KEY_F15:          return SR_KEY_F15;
            case GLFW_KEY_F16:          return SR_KEY_F16;
            case GLFW_KEY_F17:          return SR_KEY_F17;
            case GLFW_KEY_F18:          return SR_KEY_F18;
            case GLFW_KEY_F19:          return SR_KEY_F19;
            case GLFW_KEY_F20:          return SR_KEY_F20;
            case GLFW_KEY_F21:          return SR_KEY_F21;
            case GLFW_KEY_F22:          return SR_KEY_F22;
            case GLFW_KEY_F23:          return SR_KEY_F23;
            case GLFW_KEY_F24:          return SR_KEY_F24;
            case GLFW_KEY_KP_0:         return SR_KEY_NUMPAD_0;
            case GLFW_KEY_KP_1:         return SR_KEY_NUMPAD_1;
            case GLFW_KEY_KP_2:         return SR_KEY_NUMPAD_2;
            case GLFW_KEY_KP_3:         return SR_KEY_NUMPAD_3;
            case GLFW_KEY_KP_4:         return SR_KEY_NUMPAD_4;
            case GLFW_KEY_KP_5:         return SR_KEY_NUMPAD_5;
            case GLFW_KEY_KP_6:         return SR_KEY_NUMPAD_6;
            case GLFW_KEY_KP_7:         return SR_KEY_NUMPAD_7;
            case GLFW_KEY_KP_8:         return SR_KEY_NUMPAD_8;
            case GLFW_KEY_KP_9:         return SR_KEY_NUMPAD_9;
            case GLFW_KEY_KP_DECIMAL:   return SR_KEY_KP_DECIMAL;
            case GLFW_KEY_KP_DIVIDE:    return SR_KEY_KP_DIVIDE;
            case GLFW_KEY_KP_MULTIPLY:  return SR_KEY_KP_MULTIPLY;
            case GLFW_KEY_KP_SUBTRACT:  return SR_KEY_KP_SUBTRACT;
            case GLFW_KEY_KP_ADD:       return SR_KEY_KP_ADD;
            case GLFW_KEY_KP_ENTER:     return SR_KEY_KP_ENTER;
            case GLFW_KEY_KP_EQUAL:     return SR_KEY_KP_EQUAL;
            case GLFW_KEY_LEFT_SHIFT:   return SR_KEY_LEFT_SHIFT;
            case GLFW_KEY_LEFT_CONTROL: return SR_KEY_LEFT_CONTROL;
            case GLFW_KEY_LEFT_ALT:     return SR_KEY_LEFT_ALT;
            case GLFW_KEY_LEFT_SUPER:   return SR_KEY_LEFT_SUPER;
            case GLFW_KEY_RIGHT_SHIFT:  return SR_KEY_RIGHT_SHIFT;
            case GLFW_KEY_RIGHT_CONTROL:return SR_KEY_RIGHT_CONTROL;
            case GLFW_KEY_RIGHT_ALT:    return SR_KEY_RIGHT_ALT;
            case GLFW_KEY_RIGHT_SUPER:  return SR_KEY_RIGHT_SUPER;
            case GLFW_KEY_MENU:         return SR_KEY_MENU;
            default:                    return SR_KEY_NONE;
        }
    }

    SR_MOUSE_BUTTON translateGlfwMouseButton(int button) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:    return SR_MOUSE_BUTTON_LEFT;
            case GLFW_MOUSE_BUTTON_RIGHT:   return SR_MOUSE_BUTTON_RIGHT;
            case GLFW_MOUSE_BUTTON_3:       return SR_MOUSE_BUTTON_MIDDLE;
            case GLFW_MOUSE_BUTTON_4:       return SR_MOUSE_BUTTON_X1;
            case GLFW_MOUSE_BUTTON_5:       return SR_MOUSE_BUTTON_X2;
            default:                        return SR_MOUSE_BUTTON_NONE;
        }

    }


}

#endif