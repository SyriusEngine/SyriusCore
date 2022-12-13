#include "PlatformAPIX11Impl.hpp"

#if defined(SR_CORE_PLATFORM_LINUX)

namespace Syrius{

    VulkanPlatformDescX11::VulkanPlatformDescX11() {
        addExtension("VK_KHR_surface");
        addExtension("VK_KHR_xlib_surface");
    }

    GlPlatformDescX11::GlPlatformDescX11() {

    }

    PlatformAPIX11Impl::PlatformAPIX11Impl()
    : PlatformAPI(VulkanPlatformDescX11()){

    }

    PlatformAPIX11Impl::~PlatformAPIX11Impl() {

    }

    uint32 PlatformAPIX11Impl::getPrimaryScreenWidth() {
        Display* d = XOpenDisplay(nullptr);
        Screen* s = DefaultScreenOfDisplay(d);
        return s->width;
    }

    uint32 PlatformAPIX11Impl::getPrimaryScreenHeight() {
        Display* d = XOpenDisplay(nullptr);
        Screen* s = DefaultScreenOfDisplay(d);
        return s->height;
    }

    void PlatformAPIX11Impl::initPlatformGlad(GlPlatformDesc *glDesc) {
        auto glxDesc = dynamic_cast<GlPlatformDescX11*>(glDesc);

    }

    void PlatformAPIX11Impl::terminatePlatformGlad() {

    }

    SyriusWindow *PlatformAPIX11Impl::createWindow(const WindowDesc &windowDesc) {
        return nullptr;
    }

}

#endif

