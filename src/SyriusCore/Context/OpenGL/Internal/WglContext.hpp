#pragma once

#include "../GlContext.hpp"
#include "../../../../../include/SyriusCore/Dependencies/imgui/imgui.h"
#include "../../../Dependencies/imgui/imgui_impl_opengl3.h"

#if defined(SR_PLATFORM_WIN64)

#include "../../../Dependencies/imgui/imgui_impl_win32.h"
#include "../../../Dependencies/glad/glad/wgl.h"

namespace Syrius {

    class SR_API WglContext : public GlContext {
    public:
        explicit WglContext(HWND &hwnd);

        ~WglContext() override;

        void makeCurrent() override;

        void swapBuffers() override;

        void setVerticalSynchronisation(bool enable) override;

        void createImGuiContext() override;

        void destroyImGuiContext() override;

        void onImGuiBegin() override;

        void onImGuiEnd() override;

        FramebufferSize getFramebufferSize() override;

    private:
        static bool isExtensionSupported(const std::string &extName);

        void loadExtensions();

    private:

        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
        PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = NULL;

        HWND &m_Hwnd;
        HDC m_HardwareDeviceContext;
        HGLRC m_Context;

        ImGuiContext *m_ImGuiContext;

    };

}


#endif

