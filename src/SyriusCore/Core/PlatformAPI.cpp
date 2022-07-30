#include "../../../include/SyriusCore/Core/PlatformAPI.hpp"

#include <utility>
#include <vulkan/vulkan.hpp>

namespace Syrius{

    VulkanPlatformDesc::VulkanPlatformDesc() {


#if defined(SR_DEBUG_MODE)
        addRequiredLayer("VK_LAYER_KHRONOS_validation");
        addRequiredExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    }

    uint32 VulkanPlatformDesc::getExtensionCount() const {
        return m_Extensions.size();
    }

    uint32 VulkanPlatformDesc::getLayerCount() const {
        return m_ValidationLayers.size();
    }

    const std::vector<std::string> &VulkanPlatformDesc::getExtensionNames() const {
        return m_Extensions;
    }

    const std::vector<std::string> &VulkanPlatformDesc::getLayerNames() const {
        return m_ValidationLayers;
    }

    void VulkanPlatformDesc::addRequiredExtension(const std::string &extensionName) {
        m_Extensions.emplace_back(extensionName);
    }

    void VulkanPlatformDesc::addRequiredLayer(const std::string &layerName) {
        m_ValidationLayers.emplace_back(layerName);
    }

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