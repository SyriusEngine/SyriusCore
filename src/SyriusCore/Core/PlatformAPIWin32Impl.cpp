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
        return 0;
    }

    uint32 PlatformAPIWin32Impl::getPrimaryScreenHeight() {
        return 0;
    }

    void PlatformAPIWin32Impl::initPlatformGlad(GlPlatformDesc *glDesc) {

    }

    void PlatformAPIWin32Impl::terminatePlatformGlad() {

    }

}

#endif
