#include "VulkanInterface.hpp"

namespace Syrius{

    VulkanInterface::VulkanInterface(const PlatformAPI *platformAPI)
    : m_Instance(nullptr){
        m_Instance = new VulkanInstance(platformAPI);
    }

    VulkanInterface::~VulkanInterface() {
        delete m_Instance;
    }

    std::vector<std::string> VulkanInterface::getSupportedExtensions() const {
        return m_Instance->getSupportedExtensions();
    }

    std::vector<std::string> VulkanInterface::getSupportedLayers() const {
        return m_Instance->getSupportedLayers();
    }

    VulkanInstance *VulkanInterface::getInstance() const {
        return m_Instance;
    }

}