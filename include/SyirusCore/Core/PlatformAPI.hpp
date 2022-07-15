#pragma once

#include "SyriusCoreInclude.hpp"

namespace Syrius {

    struct SR_API VulkanPlatformDesc{
        uint32 m_ExtensionCount = 0;
        std::vector<std::string> m_Extensions;
    };

    struct SR_API GlPlatformDesc{
        GlPlatformDesc() = default;

        virtual ~GlPlatformDesc() = default;

    };

    class SR_API PlatformAPI{
    public:
        virtual ~PlatformAPI();

        virtual uint32 getPrimaryScreenWidth() = 0;

        virtual uint32 getPrimaryScreenHeight() = 0;

        virtual void initPlatformGlad(GlPlatformDesc* glDesc) = 0;

        virtual void terminatePlatformGlad() = 0;

        [[nodiscard]] int32 getGlVersion() const;

        [[nodiscard]] const VulkanPlatformDesc& getVulkanPlatformDesc() const;

    protected:

        explicit PlatformAPI(VulkanPlatformDesc vpDesc);

    protected:
        int32 m_GlVersion;
        uint32 m_PlatformGladInstances;

        VulkanPlatformDesc m_VulkanPlatformDesc;
    };

}



