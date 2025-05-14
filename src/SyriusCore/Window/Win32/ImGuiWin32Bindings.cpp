#include "ImGuiWin32Bindings.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius {

    constexpr static const char* s_LoggerName = "ImGuiWin32Bindings";

    bool ImGuiCreateDeviceWGL(HWND hwnd, ImGuiWglData* data) {
        HDC hDc = ::GetDC(hwnd);
        PIXELFORMATDESCRIPTOR pfd = { 0 };
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;

        const int pf = ::ChoosePixelFormat(hDc, &pfd);
        if (pf == 0) {
            SR_LOG_WARNING(s_LoggerName, "Failed to retrieve a valid PixelFormat");
            return false;
        }
        if (::SetPixelFormat(hDc, pf, &pfd) == FALSE) {
            SR_LOG_WARNING(s_LoggerName, "Failed to set PixelFormat");
            return false;
        }
        ::ReleaseDC(hwnd, hDc);

        data->hDC = ::GetDC(hwnd);
        // if (!g_hRC)
        //     g_hRC = wglCreateContext(data->hDC);
        return true;
    }

}

#endif
