#pragma once

#include "../GlContext.hpp"
#include "../../../../../include/SyriusCore/Dependencies/imgui/imgui.h"
#include "../../../Dependencies/imgui/imgui_impl_opengl3.h"

#if defined(SR_PLATFORM_WIN64)

#include "../../../Dependencies/imgui/imgui_impl_win32.h"
#include "../../../Dependencies/glad/glad/wgl.h"


namespace Syrius {

    struct ImGuiWindowData {
        HDC hDC;
        HGLRC hglrc;
    };

    class WglContext : public GlContext {
    public:
        WglContext(HWND &hwnd, const ContextDesc& desc);

        ~WglContext() override;

        void makeCurrent() override;

        void swapBuffers() override;

        void setVerticalSynchronisation(bool enable) override;

        void initImGui(const ImGuiDesc& desc) override;

        void terminateImGui() override;

        void onImGuiBegin() override;

        void onImGuiEnd() override;

    private:
        static bool isExtensionSupported(const std::string &extName);

        void loadExtensions();

        void initWGL();

        void terminateWGL();

    private:

        static void imGuiHookCreateWindow(ImGuiViewport* viewport);

        static void imGuiHookDestroyWindow(ImGuiViewport* viewport);

        static void imGuiHookRenderWindow(ImGuiViewport* viewport, void*);

        static void imGuiHookSwapBuffers(ImGuiViewport* viewport, void*);

        static bool imGuiCreateDevice(HWND hwnd, ImGuiWindowData* data);

        static void imGuiCleanupDevice(HWND hwnd, ImGuiWindowData* data);

    private:
        static u32 m_ContextCount;

        u32 m_WGLVersion;

        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
        PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = nullptr;

        HWND &m_Hwnd;
        HDC m_HardwareDeviceContext;
        HGLRC m_Context;
    };

}


#endif

