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

    struct GlPlatformDescX11: public GlPlatformDesc{

        explicit GlPlatformDescX11();

        ~GlPlatformDescX11() override = default;

    };

    class PlatformAPIX11Impl: public PlatformAPI{
    public:
        PlatformAPIX11Impl();

        ~PlatformAPIX11Impl() override;

        uint32 getPrimaryScreenWidth() override;

        uint32 getPrimaryScreenHeight() override;

        void initPlatformGlad(GlPlatformDesc* glDesc) override;

        void terminatePlatformGlad() override;

        SyriusWindow* createWindow(const WindowDesc& windowDesc) override;

        [[nodiscard]] Display* getDisplay() const;

    private:
        Display* m_Display;
        Screen* m_DefaultScreen;

    };

}

#endif

