#include "PlatformAPIWin32Impl.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    VulkanPlatformDescWin32::VulkanPlatformDescWin32() {
        m_Extensions.emplace_back("VK_KHR_surface");
        m_Extensions.emplace_back("VK_KHR_win32_surface");
        m_ExtensionCount = m_Extensions.size();
    }

    GlPlatformDescWin32::GlPlatformDescWin32(HDC hdc)
    : m_HDC(hdc){

    }

    PlatformAPIWin32Impl::PlatformAPIWin32Impl()
    : PlatformAPI(VulkanPlatformDescWin32()){

    }

    PlatformAPIWin32Impl::~PlatformAPIWin32Impl() {

    }

    uint32 PlatformAPIWin32Impl::getPrimaryScreenWidth() {
        HDC screenDC = GetDC(nullptr);
        uint32_t width = GetDeviceCaps(screenDC, HORZRES);
        ReleaseDC(nullptr, screenDC);
        return width;
    }

    uint32 PlatformAPIWin32Impl::getPrimaryScreenHeight() {
        HDC screenDC = GetDC(nullptr);
        uint32_t height = GetDeviceCaps(screenDC, VERTRES);
        ReleaseDC(nullptr, screenDC);
        return height;
    }

    void PlatformAPIWin32Impl::initPlatformGlad(GlPlatformDesc *glDesc) {
        auto wglDesc = dynamic_cast<GlPlatformDescWin32*>(glDesc);

        if (!m_PlatformGladInstances){
            m_GlVersion = gladLoaderLoadWGL(wglDesc->m_HDC);
            SR_CORE_ASSERT(m_GlVersion > 0, "Failed to initialize WGL");
        }
        m_PlatformGladInstances++;
    }

    void PlatformAPIWin32Impl::terminatePlatformGlad() {
        m_PlatformGladInstances--;
    }

    SyriusWindow *PlatformAPIWin32Impl::createWindow(const WindowDesc &windowDesc) {
        return new SyriusWindowWin32Impl(windowDesc);
    }

}

#endif
