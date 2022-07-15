#include "../../../include/SyirusCore/Core/PlatformAPI.hpp"

namespace Syrius{

    PlatformAPI::PlatformAPI(const VulkanPlatformDesc& vpDesc)
            : m_GlVersion(0),
              m_VulkanPlatformDesc(vpDesc){
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