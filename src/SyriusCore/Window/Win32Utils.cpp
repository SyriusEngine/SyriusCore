#include "Win32Utils.hpp"
#include "../Core/DebugMacros.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius {

    void setProcessDpiAware(){
        HINSTANCE shCoreDll = LoadLibraryW(L"Shcore.dll");
        if (shCoreDll){
            enum ProcessDpiAwareness
            {
                ProcessDpiUnaware         = 0,
                ProcessSystemDpiAware     = 1,
                ProcessPerMonitorDpiAware = 2
            };
            typedef HRESULT (WINAPI* SetProcessDpiAwarenessFuncType)(ProcessDpiAwareness);
            auto setProcessDpiAwarenessFunc = reinterpret_cast<SetProcessDpiAwarenessFuncType>(GetProcAddress(shCoreDll, "SetProcessDpiAwareness"));
            if (setProcessDpiAwarenessFunc){
                if (setProcessDpiAwarenessFunc(ProcessSystemDpiAware) == E_INVALIDARG){
                    SR_CORE_WARNING("Failed to set process DPI awareness using shCore.dll libary, falling back on user32.dll");
                }
                else{
                    FreeLibrary(shCoreDll);
                    return;
                }
            }
            FreeLibrary(shCoreDll);

        }
        // when setting DPI awareness using shcore.dll failed, fall back and use user32.dll and try again
        HINSTANCE user32Dll = LoadLibraryW(L"user32.dll");
        if (user32Dll){
            typedef BOOL (WINAPI* SetProcessDPIAwareFuncType)(void);
            auto setProcessDPIAwareFunc = reinterpret_cast<SetProcessDPIAwareFuncType>(GetProcAddress(user32Dll, "SetProcessDPIAware"));
            if (setProcessDPIAwareFunc){
                if (!setProcessDPIAwareFunc()){
                    SR_CORE_WARNING("Failed to set process DPI awareness");
                }
            }
            FreeLibrary(user32Dll);

        }
    }

    SR_KEYBOARD_KEY convertVirtualKey(WPARAM key, LPARAM flags){
        switch (key) {
            case VK_SHIFT: {
                unsigned int lshift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
                auto scancode = static_cast<unsigned int>((flags & (0xFF << 16)) >> 16);
                if (scancode == lshift){
                    return SR_KEY_LEFT_SHIFT;
                }
                else{
                    return SR_KEY_RIGHT_SHIFT;
                }
            }
            case VK_MENU: {
                if (HIWORD(flags) & KF_EXTENDED){
                    return SR_KEY_RIGHT_ALT;
                }
                else{
                    return SR_KEY_LEFT_ALT;
                }
            }
            case VK_CONTROL: {
                if (HIWORD(flags) & KF_EXTENDED){
                    return SR_KEY_RIGHT_CONTROL;
                }
                else{
                    return SR_KEY_LEFT_CONTROL;
                }
            }
            case VK_RCONTROL:   return SR_KEY_RIGHT_CONTROL;
            case VK_LCONTROL:   return SR_KEY_LEFT_CONTROL;
            case VK_OEM_1:      return SR_KEY_SEMICOLON;
            case VK_OEM_2:      return SR_KEY_SLASH;
            case VK_OEM_PLUS:   return SR_KEY_EQUAL;
            case VK_OEM_MINUS:  return SR_KEY_MINUS;
            case VK_OEM_4:      return SR_KEY_LEFT_BRACKET;
            case VK_OEM_6:      return SR_KEY_RIGHT_BRACKET;
            case VK_OEM_COMMA:  return SR_KEY_COMMA;
            case VK_OEM_PERIOD: return SR_KEY_PERIOD;
            case VK_OEM_7:      return SR_KEY_GRAVE_ACCENT;
            case VK_OEM_5:      return SR_KEY_BACKSLASH;
            case VK_NUMLOCK:    return SR_KEY_NUM_LOCK;
            case VK_SCROLL:     return SR_KEY_SCROLL_LOCK;
            case VK_CAPITAL:    return SR_KEY_CAPS_LOCK;
            case VK_ESCAPE:     return SR_KEY_ESCAPE;
            case VK_SPACE:      return SR_KEY_SPACE;
            case VK_SNAPSHOT:   return SR_KEY_PRINT_SCREEN;
            case VK_RETURN:     return SR_KEY_ENTER;
            case VK_BACK:       return SR_KEY_BACKSPACE;
            case VK_TAB:        return SR_KEY_TAB;
            case VK_PRIOR:      return SR_KEY_PAGE_UP;
            case VK_NEXT:       return SR_KEY_PAGE_DOWN;
            case VK_END:        return SR_KEY_END;
            case VK_HOME:       return SR_KEY_HOME;
            case VK_INSERT:     return SR_KEY_INSERT;
            case VK_DELETE:     return SR_KEY_DELETE;
            case VK_ADD:        return SR_KEY_KP_ADD;
            case VK_SUBTRACT:   return SR_KEY_KP_SUBTRACT;
            case VK_MULTIPLY:   return SR_KEY_KP_MULTIPLY;
            case VK_DIVIDE:     return SR_KEY_KP_DIVIDE;
            case VK_PAUSE:      return SR_KEY_PAUSE;
            case VK_F1:         return SR_KEY_F1;
            case VK_F2:         return SR_KEY_F2;
            case VK_F3:         return SR_KEY_F3;
            case VK_F4:         return SR_KEY_F4;
            case VK_F5:         return SR_KEY_F5;
            case VK_F6:         return SR_KEY_F6;
            case VK_F7:         return SR_KEY_F7;
            case VK_F8:         return SR_KEY_F8;
            case VK_F9:         return SR_KEY_F9;
            case VK_F10:        return SR_KEY_F10;
            case VK_F11:        return SR_KEY_F11;
            case VK_F12:        return SR_KEY_F12;
            case VK_F13:        return SR_KEY_F13;
            case VK_F14:        return SR_KEY_F14;
            case VK_F15:        return SR_KEY_F15;
            case VK_F16:        return SR_KEY_F16;
            case VK_F17:        return SR_KEY_F17;
            case VK_F18:        return SR_KEY_F18;
            case VK_F19:        return SR_KEY_F19;
            case VK_F20:        return SR_KEY_F20;
            case VK_F21:        return SR_KEY_F21;
            case VK_F22:        return SR_KEY_F22;
            case VK_F23:        return SR_KEY_F23;
            case VK_F24:        return SR_KEY_F24;
            case VK_LEFT:       return SR_KEY_LEFT;
            case VK_RIGHT:      return SR_KEY_RIGHT;
            case VK_UP:         return SR_KEY_UP;
            case VK_DOWN:       return SR_KEY_DOWN;
            case VK_NUMPAD0:    return SR_KEY_NUMPAD_0;
            case VK_NUMPAD1:    return SR_KEY_NUMPAD_1;
            case VK_NUMPAD2:    return SR_KEY_NUMPAD_2;
            case VK_NUMPAD3:    return SR_KEY_NUMPAD_3;
            case VK_NUMPAD4:    return SR_KEY_NUMPAD_4;
            case VK_NUMPAD5:    return SR_KEY_NUMPAD_5;
            case VK_NUMPAD6:    return SR_KEY_NUMPAD_6;
            case VK_NUMPAD7:    return SR_KEY_NUMPAD_7;
            case VK_NUMPAD8:    return SR_KEY_NUMPAD_8;
            case VK_NUMPAD9:    return SR_KEY_NUMPAD_9;
            case 'Q':           return SR_KEY_Q;
            case 'W':           return SR_KEY_W;
            case 'E':           return SR_KEY_E;
            case 'R':           return SR_KEY_R;
            case 'T':           return SR_KEY_T;
            case 'Y':           return SR_KEY_Y;
            case 'U':           return SR_KEY_U;
            case 'I':           return SR_KEY_I;
            case 'O':           return SR_KEY_O;
            case 'P':           return SR_KEY_P;
            case 'A':           return SR_KEY_A;
            case 'S':           return SR_KEY_S;
            case 'D':           return SR_KEY_D;
            case 'F':           return SR_KEY_F;
            case 'G':           return SR_KEY_G;
            case 'H':           return SR_KEY_H;
            case 'J':           return SR_KEY_J;
            case 'K':           return SR_KEY_K;
            case 'L':           return SR_KEY_L;
            case 'Z':           return SR_KEY_Z;
            case 'X':           return SR_KEY_X;
            case 'C':           return SR_KEY_C;
            case 'V':           return SR_KEY_V;
            case 'B':           return SR_KEY_B;
            case 'N':           return SR_KEY_N;
            case 'M':           return SR_KEY_M;
            case '0':           return SR_KEY_0;
            case '1':           return SR_KEY_1;
            case '2':           return SR_KEY_2;
            case '3':           return SR_KEY_3;
            case '4':           return SR_KEY_4;
            case '5':           return SR_KEY_5;
            case '6':           return SR_KEY_6;
            case '7':           return SR_KEY_7;
            case '8':           return SR_KEY_8;
            case '9':           return SR_KEY_9;
            default:            return SR_KEY_NONE;
        }
    }

    DWORD decodeWindowStyle(uint32_t srWinStyle){
        DWORD style = WS_VISIBLE;
        if (srWinStyle == SR_WINDOW_STYLE_DEFAULT){
            style |=  WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION;
        }
        else{
            if (srWinStyle & SR_WINDOW_STYLE_POPUP){
                style |= WS_POPUP;
            }
            else{
                if (srWinStyle & SR_WINDOW_STYLE_CLOSE){
                    style |= WS_SYSMENU;
                }
                if (srWinStyle & SR_WINDOW_STYLE_TITLEBAR){
                    style |= WS_CAPTION;
                }
                if (srWinStyle & SR_WINDOW_STYLE_RESIZE){
                    style |= WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
                }
            }
        }
        return style;
    }

    DWORD decodeExtendedWindowStyle(uint32_t srWinStyle){
        DWORD style = 0;
        if (srWinStyle == SR_WINDOW_STYLE_DEFAULT){
            style |= WS_EX_ACCEPTFILES;
        }
        else{
            if (srWinStyle & SR_WINDOW_STYLE_FILE_DROP){
                style |= WS_EX_ACCEPTFILES;
            }
            if (srWinStyle & SR_WINDOW_STYLE_SUNKEN_EDGE){
                style |= WS_EX_CLIENTEDGE;
            }
            if (srWinStyle & SR_WINDOW_STYLE_RAISED_EDGE){
                style |= WS_EX_WINDOWEDGE;
            }
        }
        return style;
    }

    std::string wideToNormalString(const wchar_t* source){
        char buffer[1024];
        wcstombs(buffer, source, sizeof(buffer));
        std::string str(buffer);
        return str;
    }

    std::wstring stringToWideString(const std::string& source){
        int len;
        int slength = (int)source.length() + 1;
        len = MultiByteToWideChar(CP_ACP, 0, source.c_str(), slength, 0, 0);
        auto* buf = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, source.c_str(), slength, buf, len);
        std::wstring r(buf);
        delete[] buf;
        return r;
    }

}

#endif
