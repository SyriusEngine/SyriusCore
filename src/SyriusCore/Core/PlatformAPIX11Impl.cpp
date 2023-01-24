#include "PlatformAPIX11Impl.hpp"
#include "../Window/SyriusWindowX11Impl.hpp"
#include "../Dependencies/glad/glad/glx.h"

#if defined(SR_CORE_PLATFORM_LINUX)

namespace Syrius{

    VulkanPlatformDescX11::VulkanPlatformDescX11() {
        addExtension("VK_KHR_surface");
        addExtension("VK_KHR_xlib_surface");
    }

    GlPlatformDescX11::GlPlatformDescX11() {

    }

    PlatformAPIX11Impl::PlatformAPIX11Impl()
    : PlatformAPI(VulkanPlatformDescX11()),
    m_Display(XOpenDisplay(nullptr)),
    m_GladInstanceCount(0) {
        // first thing, set the error handler
        XSetErrorHandler(&DebugMessageHandler::x11ErrorHandler);

        SR_CORE_PRECONDITION(m_Display != nullptr, "Failed to connect to Xserver Display")
        m_DefaultScreen = DefaultScreenOfDisplay(m_Display);


    }

    PlatformAPIX11Impl::~PlatformAPIX11Impl() {
        XCloseDisplay(m_Display);
    }

    uint32 PlatformAPIX11Impl::getPrimaryScreenWidth() {
        return m_DefaultScreen->width;
    }

    uint32 PlatformAPIX11Impl::getPrimaryScreenHeight() {
        return m_DefaultScreen->height;
    }

    void PlatformAPIX11Impl::initPlatformGlad(GlPlatformDesc *glDesc) {
        auto glxDesc = dynamic_cast<GlPlatformDescX11*>(glDesc);
        if (m_GladInstanceCount == 0){
            if (glxDesc != nullptr){
                int32 glxVersion = gladLoaderLoadGLX(m_Display, DefaultScreen(m_Display));
                SR_CORE_ASSERT(glxVersion > 0, "Failed to initialize GLX");
                SR_CORE_MESSAGE("GLX initialized with version : %i.%i", GLAD_VERSION_MAJOR(glxVersion), GLAD_VERSION_MINOR(glxVersion));
            }
            else{
                SR_CORE_EXCEPTION("Failed to cast GlPlatformDesc to GlPlatformDescX11");
            }
        }
        m_GladInstanceCount++;
    }

    void PlatformAPIX11Impl::terminatePlatformGlad() {
        m_GladInstanceCount--;
        if (m_GladInstanceCount == 0){
            gladLoaderUnloadGLX();
        }

    }

    Resource<SyriusWindow> PlatformAPIX11Impl::createWindow(const WindowDesc &windowDesc, CoreCommand* coreCommand) {
        return Resource<SyriusWindow>new SyriusWindowX11Impl(windowDesc, m_Display, coreCommand));
    }

    Display *PlatformAPIX11Impl::getDisplay() const {
        return m_Display;
    }

}

#endif

