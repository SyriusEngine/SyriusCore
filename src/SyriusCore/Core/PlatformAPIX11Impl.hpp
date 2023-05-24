#pragma once

#include "../../../../include/SyriusCore/Core/PlatformAPI.hpp"
#include "PlatformInclude.hpp"
#include "DebugMacros.hpp"


#if defined(SR_CORE_PLATFORM_LINUX)

namespace Syrius{

    struct VulkanPlatformDescX11: public VulkanPlatformDesc{
    public:

        VulkanPlatformDescX11();
    };

    class PlatformAPIX11Impl: public PlatformAPI{
    public:
        PlatformAPIX11Impl();

        ~PlatformAPIX11Impl() override;

        uint32 getPrimaryScreenWidth() override;

        uint32 getPrimaryScreenHeight() override;

        void initPlatformGlad();

        void terminatePlatformGlad() override;

        Resource<SyriusWindow> createWindow(const WindowDesc& windowDesc) override;

        [[nodiscard]] Display* getDisplay() const;

    private:
        Display* m_Display;
        Screen* m_DefaultScreen;

        uint32 m_GladInstanceCount;

    };

}

#endif

