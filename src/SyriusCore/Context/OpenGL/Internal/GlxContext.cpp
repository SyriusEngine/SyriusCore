#include "GlxContext.hpp"

#include "../../../Core/CoreCommand.hpp"
#include "../../../Core/PlatformAPIX11Impl.hpp"

#if defined(SR_CORE_PLATFORM_LINUX)

namespace Syrius{

    GlxContext::GlxContext(Display *display, Window &window, const ContextDesc &desc)
    : GlContext(desc),
    m_Display(display),
    m_Window(window){
        // create a new window
        XDestroyWindow(m_Display, m_Window);

        auto glxDesc = new GlPlatformDescX11();
        CoreCommand::initPlatformGlad(glxDesc);
        delete glxDesc;

        // describe the desired framebuffer
        int32 contextAttribs[] = {
                GLX_RGBA, GLX_DOUBLEBUFFER,
//                GLX_RED_SIZE, desc.m_RedBits,
//                GLX_GREEN_SIZE, desc.m_GreenBits,
//                GLX_BLUE_SIZE, desc.m_BlueBits,
//                GLX_ALPHA_SIZE, desc.m_AlphaBits,
//                GLX_DEPTH_SIZE, desc.m_DepthBits,
//                GLX_STENCIL_SIZE, desc.m_StencilBits,
                None
        };
        Window root = DefaultRootWindow(m_Display);
        XVisualInfo* visualInfo = glXChooseVisual(m_Display, 0, contextAttribs);
        SR_CORE_ASSERT(visualInfo, "Failed to choose visual");

        XSetWindowAttributes windowAttribs = {};
        windowAttribs.colormap = XCreateColormap(m_Display, root, visualInfo->visual, AllocNone);
        windowAttribs.event_mask = FocusChangeMask      | ButtonPressMask     |
                                   ButtonReleaseMask    | ButtonMotionMask    |
                                   PointerMotionMask    | KeyPressMask        |
                                   KeyReleaseMask       | StructureNotifyMask |
                                   EnterWindowMask      | LeaveWindowMask     |
                                   VisibilityChangeMask | PropertyChangeMask;
        m_Window = XCreateWindow(m_Display, root,
                                 desc.m_DefaultFrameBufferDesc.m_XPos, desc.m_DefaultFrameBufferDesc.m_YPos,
                                 desc.m_DefaultFrameBufferDesc.m_Width, desc.m_DefaultFrameBufferDesc.m_Height,
                                 0, visualInfo->depth, InputOutput, visualInfo->visual, CWColormap | CWEventMask, &windowAttribs);

        // create the context
        m_Context = glXCreateContext(m_Display, visualInfo, nullptr, GL_TRUE);
        SR_CORE_ASSERT(m_Context, "Failed to create context");
        glXMakeCurrent(m_Display, m_Window, m_Context);

        XFree(visualInfo);

        initGl(desc.m_DefaultFrameBufferDesc);
    }

    GlxContext::~GlxContext() {
        glXMakeCurrent(m_Display, None, nullptr);
        glXDestroyContext(m_Display, m_Context);
    }

    void GlxContext::makeCurrent() {
        glXMakeCurrent(m_Display, m_Window, m_Context);
    }

    void GlxContext::swapBuffers() {
        glXSwapBuffers(m_Display, m_Window);
    }

    void GlxContext::setVerticalSynchronisation(bool enable) {
        if (enable){
            glXSwapIntervalEXT(m_Display, m_Window, 1);
        } else {
            glXSwapIntervalEXT(m_Display, m_Window, 0);
        }
    }

    FramebufferSize GlxContext::getFramebufferSize() {
        XWindowAttributes windowAttribs;
        XGetWindowAttributes(m_Display, m_Window, &windowAttribs);
        FramebufferSize size;
        size.m_Width = windowAttribs.width;
        size.m_Height = windowAttribs.height;
        return size;
    }

    void GlxContext::createImGuiContext() {

    }

    void GlxContext::destroyImGuiContext() {

    }

    void GlxContext::onImGuiBegin() {

    }

    void GlxContext::onImGuiEnd() {

    }
}

#endif

