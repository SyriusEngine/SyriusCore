#pragma once

#include "../../../../include/SyriusCore/Core/PlatformAPI.hpp"
#include "PlatformInclude.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    static const wchar_t* s_SyriusWindowClass = L"SYRIUS_CORE_API";


    struct VulkanPlatformDescWin32: public VulkanPlatformDesc{
    public:

        VulkanPlatformDescWin32();
    };

    class PlatformAPIWin32Impl: public PlatformAPI{
    public:

        PlatformAPIWin32Impl();

        ~PlatformAPIWin32Impl() override;

        uint32 getPrimaryScreenWidth() override;

        uint32 getPrimaryScreenHeight() override;

        void initPlatformGlad(HDC hdc);

        void terminatePlatformGlad() override;

        Resource<SyriusWindow> createWindow(const WindowDesc& windowDesc) override;

    private:

        void setProcessDpiAware();

    };

}

#endif

