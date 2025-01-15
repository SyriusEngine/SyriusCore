#pragma once

#include "CoreLoggerDefs.hpp"

#if !defined(SR_PLATFORM_WIN64)
#include <GLFW/glfw3.h>
#endif

namespace Syrius{

    class CoreLogger{
    public:

        static void openGLMessageCallback(GLenum source, u32 type, u32 id, u32 severity, i32 length, const char* message, const void* userParam);

        static void openGLError(GLenum error, const std::string& function, const std::string& file, u32 line);

        static void openGLClearError(const std::string& function, const std::string& file, u32 line);

        static void openGLClearErrorNoLog();


#if defined(SR_PLATFORM_WIN64)
        static void formatHresultMessage(HRESULT hr, const std::string& function, const std::string& file, u32 line);

        static void dxgiLogDeviceRemoved(HRESULT hr, const std::string& function, const std::string& file, u32 line);

#if defined(SR_COMPILER_MSVC)
        static void dxgiGetMessages();
#endif

#else
        static void glfwCallback(int code, const char* description);

#endif

    private:
#if defined(SR_COMPILER_MSVC)
        static u64 m_DxgiMessageIndex;
        static struct IDXGIInfoQueue* m_DxgiInfoQueue;
#endif

    };

}
