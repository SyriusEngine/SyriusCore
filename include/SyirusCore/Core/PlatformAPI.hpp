#pragma once

#include "SyriusCoreInclude.hpp"

namespace Syrius {

    struct SR_API VulkanPlatformDesc{
        uint32 m_ExtensionCount = 0;
        std::vector<std::string> m_Extensions;
    };

    class SR_API PlatformAPI{
    public:
        virtual ~PlatformAPI();

        virtual uint32 getPrimaryScreenWidth() = 0;

        virtual uint32 getPrimaryScreenHeight() = 0;

#if defined(SR_PLATFORM_WIN64)
        virtual void initPlatformGlad(HDC hdc) = 0;
#endif
        virtual void terminatePlatformGlad() = 0;

        [[nodiscard]] int32 getGlVersion() const;

        [[nodiscard]] const VulkanPlatformDesc& getVulkanPlatformDesc() const;

    protected:

        explicit PlatformAPI(const VulkanPlatformDesc& vpDesc);

    protected:
        int32 m_GlVersion;

    private:
        VulkanPlatformDesc m_VulkanPlatformDesc;
    };

}



