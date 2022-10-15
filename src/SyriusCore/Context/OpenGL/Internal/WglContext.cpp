#include "WglContext.hpp"
#include "../../../Core/CoreCommand.hpp"
#include "../../../Core/PlatformAPIWin32Impl.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    WglContext::WglContext(HWND &hwnd, const ContextDesc& desc)
    : GlContext(desc),
    m_Hwnd(hwnd),
    m_Context(nullptr),
    m_HardwareDeviceContext(nullptr),
    m_ImGuiContext(nullptr){
        m_HardwareDeviceContext = GetDC(m_Hwnd);

        uint8_t pixelType = desc.m_RedBits + desc.m_GreenBits + desc.m_BlueBits + desc.m_AlphaBits;

        DWORD pixelFormatFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        PIXELFORMATDESCRIPTOR pixelDesc = {
                sizeof(pixelDesc),
                1,
                pixelFormatFlags,
                pixelType,
                0,
                desc.m_RedBits,
                0,
                desc.m_GreenBits,
                0,
                desc.m_BlueBits,
                0,
                desc.m_AlphaBits,
                0,
                0,
                0,
                0,
                0,
                0,
                desc.m_DepthBits,
                desc.m_StencilBits,
                PFD_MAIN_PLANE,
                0,
                0,
                0,
                0
        };

        int32 tempPixelFormat = ChoosePixelFormat(m_HardwareDeviceContext, &pixelDesc);
        bool pfRes = SetPixelFormat(m_HardwareDeviceContext, tempPixelFormat, &pixelDesc);
        SR_CORE_ASSERT(pfRes, "Failed to set pixel format");

        //create a throwaway context to get the function pointers
        HGLRC tempContext = wglCreateContext(m_HardwareDeviceContext);
        wglMakeCurrent(m_HardwareDeviceContext, tempContext);

        auto gDesc = new GlPlatformDescWin32(m_HardwareDeviceContext);
        CoreCommand::initPlatformGlad(gDesc);
        delete gDesc;

        CoreCommand::init();

        // create the actual context
        const int attribList[] =
            {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB, pixelType,
                WGL_DEPTH_BITS_ARB, desc.m_DepthBits,
                WGL_STENCIL_BITS_ARB, desc.m_StencilBits,
                0, // End
            };

        int pixelFormat;
        UINT numFormats;

        BOOL result = wglChoosePixelFormatARB(m_HardwareDeviceContext, attribList, nullptr, 1, &pixelFormat, &numFormats);
        SR_CORE_ASSERT(result, "Failed to choose pixel format");
        int attributes[] =
            {
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
        CoreCommand::terminateGlad();

        initGl(desc.m_DefaultFrameBufferDesc);
        loadExtensions();

        SR_CORE_POSTCONDITION(m_HardwareDeviceContext, "Failed to create hardware device context")
        SR_CORE_POSTCONDITION(m_Context, "Failed to create context")
    }

    WglContext::~WglContext() {
        wglDeleteContext(m_Context);
        CoreCommand::terminatePlatformGlad();
    }

    void WglContext::makeCurrent() {
        wglMakeCurrent(m_HardwareDeviceContext, m_Context);
    }

    void WglContext::swapBuffers() {
        SwapBuffers(m_HardwareDeviceContext);
    }

    void WglContext::setVerticalSynchronisation(bool enable) {
        SR_CORE_PRECONDITION(wglSwapIntervalEXT != nullptr, "Cannot set vertical synchronisation, extension not supported")

        m_VerticalSync = enable;
        wglSwapIntervalEXT(m_VerticalSync);
    }

    void WglContext::createImGuiContext() {
        SR_CORE_PRECONDITION(!m_ImGuiContext, "There exists already an ImGui context")

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(m_Hwnd);
        CoreCommand::initGlad();
        ImGui_ImplOpenGL3_Init("#version 150");

        m_ImGuiContext = ImGui::GetCurrentContext();

        SR_CORE_PRECONDITION(m_ImGuiContext, "Failed to create ImGui context");
    }

    void WglContext::destroyImGuiContext() {
        SR_CORE_PRECONDITION(m_ImGuiContext, "There does not exists an ImGui context");

        ImGui::SetCurrentContext(m_ImGuiContext);
        ImGui_ImplOpenGL3_Shutdown();
        CoreCommand::terminateGlad();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        m_ImGuiContext = nullptr;
    }

    void WglContext::onImGuiBegin() {
        SR_CORE_PRECONDITION(m_ImGuiContext, "There does not exists an ImGui context");

        ImGui::SetCurrentContext(m_ImGuiContext);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void WglContext::onImGuiEnd() {
        SR_CORE_PRECONDITION(m_ImGuiContext, "There does not exists an ImGui context");

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    bool WglContext::isExtensionSupported(const std::string &extName) {
        PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = nullptr;
        _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) wglGetProcAddress("wglGetExtensionsStringEXT");

        if (strstr(_wglGetExtensionsStringEXT(), extName.c_str()) == nullptr) {
            return false;
        }
        else{
            return true;
        }
    }

    void WglContext::loadExtensions() {
        if (isExtensionSupported("WGL_EXT_swap_control")) {
            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
            wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress("wglGetSwapIntervalEXT");
        }
        else{
            SR_CORE_WARNING("extension: <WGL_EXT_swap_control> is not supported")

        }
    }

    FramebufferSize WglContext::getFramebufferSize() {
        RECT area;
        GetClientRect(m_Hwnd, &area);
        FramebufferSize size;
        size.m_Width = area.right;
        size.m_Height = area.bottom;
        return size;
    }

}

#endif
