#include "VulkanLogicalDevice.hpp"

namespace Syrius{


    VulkanLogicalDevice::VulkanLogicalDevice(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice)
    : m_Device(nullptr),
    m_GraphicsQueue(nullptr),
    m_Instance(instance),
    m_PhysicalDevice(physicalDevice){
        QueueFamilyIndices indices = findQueueFamily(m_PhysicalDevice->getDevice());
        float queuePriority = 1.0f;
        auto validationLayers = m_Instance->getSupportedLayers();

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.m_GraphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;

        SR_VULKAN_CALL(vkCreateDevice(m_PhysicalDevice->getDevice(), &createInfo, nullptr, &m_Device), "Failed to create logical device");

        vkGetDeviceQueue(m_Device, indices.m_GraphicsFamily.value(), 0, &m_GraphicsQueue);

    }

    VulkanLogicalDevice::~VulkanLogicalDevice() {
        if (m_Device){
            vkDestroyDevice(m_Device, nullptr);
        }
    }

    VkDevice &VulkanLogicalDevice::getDevice() {
        return m_Device;
    }
}
