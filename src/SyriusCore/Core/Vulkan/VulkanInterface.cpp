#include "VulkanInterface.hpp"

namespace Syrius{

    VulkanInterface::VulkanInterface(const PlatformAPI *platformAPI)
    : m_Instance(nullptr),
      m_PhysicalDevice(nullptr),
      m_LogicalDevice(nullptr){
        m_Instance = new VulkanInstance(platformAPI);
    }

    VulkanInterface::~VulkanInterface() {
        delete m_LogicalDevice;
        delete m_PhysicalDevice;
        delete m_Instance;
    }

    std::vector<std::string> VulkanInterface::getSupportedExtensions() const {
        return m_Instance->getSupportedExtensions();
    }

    std::vector<std::string> VulkanInterface::getSupportedLayers() const {
        return m_Instance->getSupportedLayers();
    }

    void VulkanInterface::createDevices() {
        if (m_PhysicalDevice == nullptr){
            m_PhysicalDevice = new VulkanPhysicalDevice(m_Instance->getInstance());
            m_LogicalDevice = new VulkanLogicalDevice(m_Instance, m_PhysicalDevice);
        }
    }

}