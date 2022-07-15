#pragma once

#include "../../../../include/SyirusCore/Core/PlatformAPI.hpp"
#include "PlatformInclude.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    struct VulkanPlatformDescWin32: public VulkanPlatformDesc{
    public:

        VulkanPlatformDescWin32();
    };

    struct GlPlatformDescWin32: public GlPlatformDesc{
        HDC m_HDC;

        explicit GlPlatformDescWin32(HDC hdc);

        ~GlPlatformDescWin32() override = default;

    };

    class PlatformAPIWin32Impl: public PlatformAPI{
    public:

        PlatformAPIWin32Impl();

        ~PlatformAPIWin32Impl() override;

        uint32 getPrimaryScreenWidth() override;

        uint32 getPrimaryScreenHeight() override;

        void initPlatformGlad(GlPlatformDesc* glDesc) override;

        void terminatePlatformGlad() override;
    };

}

#endif

