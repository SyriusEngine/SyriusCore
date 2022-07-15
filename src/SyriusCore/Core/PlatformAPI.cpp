#include "../../../include/SyirusCore/Core/PlatformAPI.hpp"

#include <utility>

namespace Syrius{

    PlatformAPI::PlatformAPI(VulkanPlatformDesc vpDesc)
            : m_GlVersion(0),
              m_PlatformGladInstances(0),
              m_VulkanPlatformDesc(std::move(vpDesc)){
    }

    PlatformAPI::~PlatformAPI() {
    }

    int32 PlatformAPI::getGlVersion() const {
        return m_GlVersion;
    }

    const VulkanPlatformDesc &PlatformAPI::getVulkanPlatformDesc() const {
        return m_VulkanPlatformDesc;
    }


}