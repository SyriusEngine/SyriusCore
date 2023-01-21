#include "GlxContext.hpp"

#include "../../../Core/CoreCommand.hpp"
#include "../../../Core/PlatformAPIX11Impl.hpp"

#if defined(SR_CORE_PLATFORM_LINUX)

namespace Syrius{

    GlxContext::GlxContext(Display *display, GLXFBConfig fbConfig, Window& window, const ContextDesc &desc)
    : GlContext(desc),
    m_Display(display),
    m_Window(window){
        auto glxDesc = new GlPlatformDescX11();
        CoreCommand::initPlatformGlad(glxDesc);
        delete glxDesc;

        int32 contextAttribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
            GLX_CONTEXT_MINOR_VERSION_ARB, 6,
            None
        };

        m_Context = glXCreateContextAttribsARB(m_Display, fbConfig, 0, True, contextAttribs);
        if(!m_Context){
            // fall back to old-style 2.x context
            SR_CORE_WARNING("Failed to create OpenGL 4.6 context. Falling back to OpenGL 2.1 context.");
            m_Context = glXCreateNewContext(m_Display, fbConfig, GLX_RGBA_TYPE, 0, True);
            if (!m_Context){
                SR_CORE_WARNING("Failed to create OpenGL 2.1 context.");
            }
        }

        glXMakeContextCurrent(m_Display, m_Window, m_Window, m_Context);

        initGl(desc);
    }

    GlxContext::~GlxContext() {
        glXMakeContextCurrent(m_Display, None, None, nullptr);
        glXDestroyContext(m_Display, m_Context);

        CoreCommand::terminatePlatformGlad();
    }

    void GlxContext::makeCurrent() {
        glXMakeContextCurrent(m_Display, m_Window, m_Window, m_Context);
    }

    void GlxContext::swapBuffers() {
        glXSwapBuffers(m_Display, m_Window);
    }

    void GlxContext::setVerticalSynchronisation(bool enable) {
        if(enable){
            glXSwapIntervalEXT(m_Display, m_Window, 1);
        }else{
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

