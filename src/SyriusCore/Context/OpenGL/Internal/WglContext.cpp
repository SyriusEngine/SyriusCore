#include "WglContext.hpp"

#include "../../../../include/SyriusCore/Dependencies/imgui/implot.h"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    static std::string s_ImGuiLoggerName = "ImGuiWglContext";

    u32 WglContext::m_ContextCount = 0;

    WglContext::WglContext(HWND &hwnd, const ContextDesc& desc):
    GlContext(desc),
    m_Hwnd(hwnd),
    m_Context(nullptr),
    m_HardwareDeviceContext(nullptr),
    m_WGLVersion(0){
        m_HardwareDeviceContext = GetDC(m_Hwnd);

        const u8 colorBits = desc.redBits + desc.greenBits + desc.blueBits + desc.alphaBits;

        const DWORD pixelFormatFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        PIXELFORMATDESCRIPTOR pixelDesc = {
                sizeof(pixelDesc),
                1,
                pixelFormatFlags,
                PFD_TYPE_RGBA,
                colorBits,
                desc.redBits,
                0,
                desc.greenBits,
                0,
                desc.blueBits,
                0,
                desc.alphaBits,
                0,
                0,
                0,
                0,
                0,
                0,
                desc.depthBits,
                desc.stencilBits,
                PFD_MAIN_PLANE,
                0,
                0,
                0,
                0
        };

        const i32 tempPixelFormat = ChoosePixelFormat(m_HardwareDeviceContext, &pixelDesc);
        bool pfRes = SetPixelFormat(m_HardwareDeviceContext, tempPixelFormat, &pixelDesc);

        //create a throwaway context to get the function pointers
        HGLRC tempContext = wglCreateContext(m_HardwareDeviceContext);
        wglMakeCurrent(m_HardwareDeviceContext, tempContext);

        initWGL();

        if (wglCreateContextAttribsARB != nullptr){
            // create the actual context
            const int attribList[] = {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
#if defined(SR_DEBUG)
                WGL_CONTEXT_DEBUG_BIT_ARB,		GL_TRUE,
#else
                WGL_CONTEXT_DEBUG_BIT_ARB,		GL_FALSE,
#endif
                WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB, colorBits,
                WGL_DEPTH_BITS_ARB, desc.depthBits,
                WGL_STENCIL_BITS_ARB, desc.stencilBits,
                0, // End
            };

            int pixelFormat;
            UINT numFormats;

            BOOL result = wglChoosePixelFormatARB(m_HardwareDeviceContext, attribList, nullptr, 1, &pixelFormat, &numFormats);
            SR_LOG_WARNING_IF_FALSE(result, "WglContext", "Failed to choose pixel format");
            int attributes[] ={
                WGL_CONTEXT_MAJOR_VERSION_ARB,	4,
                WGL_CONTEXT_MINOR_VERSION_ARB,	6,
                WGL_CONTEXT_PROFILE_MASK_ARB,	WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0 // End of attributes
            };
            m_Context = wglCreateContextAttribsARB(m_HardwareDeviceContext, nullptr, attributes);
            // set the actual context as the current context
            wglMakeCurrent(m_HardwareDeviceContext, m_Context);

            // delete the throwaway context
            wglDeleteContext(tempContext);

            SR_LOG_INFO("WglContext", "Created OpenGL context with version: {}.{}", attributes[1], attributes[3]);
        }
        else{
            // use the throwaway context as the actual context
            m_Context = tempContext;
            SR_LOG_INFO("WglContext", "OpenGL context created without extensions");
        }

        loadExtensions();
        initGl();
        RECT clientSpace = {0, 0, 0, 0};
        GetClientRect(m_Hwnd, &clientSpace);
        const i32 width = clientSpace.right;
        const i32 height = clientSpace.bottom;
        GlContext::createDefaultFrameBuffer(width, height, desc);

        SR_POSTCONDITION(m_HardwareDeviceContext, "Failed to create hardware device context")
        SR_POSTCONDITION(m_Context, "Failed to create context")
    }

    WglContext::~WglContext() {
        terminateGl();
        wglDeleteContext(m_Context);
        terminateWGL();
    }

    void WglContext::makeCurrent() {
        wglMakeCurrent(m_HardwareDeviceContext, m_Context);
    }

    void WglContext::swapBuffers() {
        SwapBuffers(m_HardwareDeviceContext);
    }

    void WglContext::setVerticalSynchronisation(bool enable) {
        SR_PRECONDITION(wglSwapIntervalEXT != nullptr, "Cannot set vertical synchronisation, extension not supported")

        m_VerticalSync = enable;
        wglSwapIntervalEXT(m_VerticalSync);
    }

    void WglContext::initImGui(const ImGuiDesc& desc) {
        Context::initImGui(desc);
        if (desc.useDocking) {
            ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
            platform_io.Renderer_CreateWindow = WglContext::imGuiHookCreateWindow;
            platform_io.Renderer_DestroyWindow = WglContext::imGuiHookDestroyWindow;
            platform_io.Renderer_SwapBuffers = WglContext::imGuiHookRenderWindow;
            platform_io.Platform_RenderWindow = WglContext::imGuiHookRenderWindow;
        }

        ImGui_ImplWin32_Init(m_Hwnd);
        ImGui_ImplOpenGL3_Init("#version 150");
    }

    void WglContext::terminateImGui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();

        Context::terminateImGui();
    }

    void WglContext::onImGuiBegin() {
        SR_PRECONDITION(m_ImGuiContextCreated == true, "There does not exists an ImGui context");
        SR_PRECONDITION(m_IsImGuiRendering == false, "ImGuiRendering already started!")

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        m_IsImGuiRendering = true;

        SR_POSTCONDITION(m_IsImGuiRendering == true, "Failed to start ImGuiRendering");
    }

    void WglContext::onImGuiEnd() {
        SR_PRECONDITION(m_IsImGuiRendering == true, "onImGuiEnd() called before onImGuiBegin()")

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            // Restore the OpenGL rendering context to the main window DC, since platform windows might have changed it.
            makeCurrent();
        }
        m_IsImGuiRendering = false;
    }

    bool WglContext::isExtensionSupported(const std::string &extName) {
        PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = nullptr;
        _wglGetExtensionsStringEXT = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGEXTPROC>(wglGetProcAddress("wglGetExtensionsStringEXT"));

        if (strstr(_wglGetExtensionsStringEXT(), extName.c_str()) == nullptr) {
            return false;
        }
        else{
            return true;
        }
    }

    void WglContext::loadExtensions() {
        if (isExtensionSupported("WGL_EXT_swap_control")) {
            wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
            wglGetSwapIntervalEXT = reinterpret_cast<PFNWGLGETSWAPINTERVALEXTPROC>(wglGetProcAddress("wglGetSwapIntervalEXT"));
        }
        else{
            SR_LOG_WARNING("WglContext", "extension: <WGL_EXT_swap_control> is not supported")

        }
    }

    void WglContext::initWGL() {
        if (m_ContextCount == 0){
            m_WGLVersion = gladLoaderLoadWGL(m_HardwareDeviceContext);
            SR_ASSERT(m_WGLVersion > 0, "Failed to initialize WGL");
            SR_LOG_INFO("WglContext", "Initialized WGL version: {}", m_WGLVersion);
        }
        m_ContextCount++;

    }

    void WglContext::terminateWGL() {
        m_ContextCount--;
        if (m_ContextCount == 0){
//            gladLoaderUnloadWGL(); TODO: fix this
        }
    }

    void WglContext::imGuiHookCreateWindow(ImGuiViewport *viewport) {
        assert(viewport->RendererUserData == NULL);

        auto* data = IM_NEW(ImGuiWindowData);
        imGuiCreateDevice(static_cast<HWND>(viewport->PlatformHandle), data);
        viewport->RendererUserData = data;
    }

    void WglContext::imGuiHookDestroyWindow(ImGuiViewport *viewport) {
        if (viewport->RendererUserData != nullptr){
            auto* data = static_cast<ImGuiWindowData *>(viewport->RendererUserData);
            imGuiCleanupDevice(static_cast<HWND>(viewport->PlatformHandle), data);
            IM_DELETE(data);
            viewport->RendererUserData = nullptr;
        }
    }

    void WglContext::imGuiHookRenderWindow(ImGuiViewport *viewport, void*) {
        auto* data = static_cast<ImGuiWindowData *>(viewport->RendererUserData);
        if (data) {
            wglMakeCurrent(data->hDC, data->hglrc);
        }
    }

    void WglContext::imGuiHookSwapBuffers(ImGuiViewport *viewport, void*) {
        auto* data = static_cast<ImGuiWindowData *>(viewport->RendererUserData);
        if (data) {
            SwapBuffers(data->hDC);
        }
    }

    bool WglContext::imGuiCreateDevice(HWND hwnd, ImGuiWindowData *data) {
        HDC hDc = ::GetDC(hwnd);
        PIXELFORMATDESCRIPTOR pfd = { 0 };
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;

        const int pf = ChoosePixelFormat(hDc, &pfd);
        if (pf == 0) {
            SR_LOG_WARNING(s_ImGuiLoggerName, "Failed to choose a pixel format");
            ReleaseDC(hwnd, hDc);
            return false;
        }
        if (SetPixelFormat(hDc, pf, &pfd) == FALSE) {
            SR_LOG_WARNING(s_ImGuiLoggerName, "Failed to set the pixel format");
            ReleaseDC(hwnd, hDc);
            return false;
        }
        HGLRC hglrc = wglCreateContext(hDc);
        if (!hglrc) {
            SR_LOG_WARNING(s_ImGuiLoggerName, "Failed to create OpenGL context for viewport window.");
            ReleaseDC(hwnd, hDc);
            return false;
        }

        data->hDC = GetDC(hwnd);
        data->hglrc = hglrc;
        return true;
    }

    void WglContext::imGuiCleanupDevice(HWND hwnd, ImGuiWindowData *data) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(data->hglrc);
        ReleaseDC(hwnd, data->hDC);
    }
}

#endif
