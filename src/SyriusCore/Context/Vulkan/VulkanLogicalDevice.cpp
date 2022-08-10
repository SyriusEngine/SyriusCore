#include "VulkanLogicalDevice.hpp"

namespace Syrius{

    VulkanLogicalDevice::VulkanLogicalDevice(VulkanPhysicalDevice *physicalDevice, VkSurfaceKHR surface)
    : m_PhysicalDevice(physicalDevice),
    m_Device(nullptr),
    m_GraphicsQueue(nullptr),
    m_PresentQueue(nullptr){

        QueueFamilyIndices indices = findQueueFamily(m_PhysicalDevice->getPhysicalDevice(), surface);
        float queuePriority = 1.0f;
        auto validationLayers = CoreCommand::getVulkanInstance()->getSupportedLayers();

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32> uniqueQueueFamilies = {indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value()};
        for (uint32 queueFamily : uniqueQueueFamilies){
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};


        auto extensionNames = m_PhysicalDevice->getExtensionsNames();
        const char** extensions = new const char*[extensionNames.size()];
        for (uint32 i = 0; i < extensionNames.size(); i++){
            extensions[i] = extensionNames[i].c_str();
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = queueCreateInfos.size();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = extensionNames.size();
        createInfo.ppEnabledExtensionNames = extensions;

        SR_VULKAN_CALL(vkCreateDevice(m_PhysicalDevice->getPhysicalDevice(), &createInfo, nullptr, &m_Device), "Failed to create logical device");

        vkGetDeviceQueue(m_Device, indices.m_GraphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, indices.m_PresentFamily.value(), 0, &m_PresentQueue);

        SR_CORE_POSTCONDITION(m_Device != nullptr, "Failed to create logical device");
    }

    VulkanLogicalDevice::~VulkanLogicalDevice() {
        if (m_Device != nullptr){
            vkDestroyDevice(m_Device, nullptr);
        }
    }

    VkDevice VulkanLogicalDevice::getDevice() {
        return m_Device;
    }
}
