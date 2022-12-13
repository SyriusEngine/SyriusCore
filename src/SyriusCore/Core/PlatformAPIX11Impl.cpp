#include "PlatformAPIX11Impl.hpp"
#include "../Window/SyriusWindowX11Impl.hpp"

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
    m_Display(XOpenDisplay(nullptr)){
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

    }

    void PlatformAPIX11Impl::terminatePlatformGlad() {

    }

    SyriusWindow *PlatformAPIX11Impl::createWindow(const WindowDesc &windowDesc) {
        return new SyriusWindowX11Impl(windowDesc, m_Display);
    }

    Display *PlatformAPIX11Impl::getDisplay() const {
        return m_Display;
    }

}

#endif

