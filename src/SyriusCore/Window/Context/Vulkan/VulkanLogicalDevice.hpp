#pragma once

#include "VulkanUtils.hpp"
#include "VulkanPhysicalDevice.hpp"

namespace Syrius{

    class VulkanLogicalDevice{
    public:

        VulkanLogicalDevice(VulkanPhysicalDevice* physicalDevice, VkSurfaceKHR surface);

        ~VulkanLogicalDevice();

        VkDevice getDevice();

    private:
        VulkanPhysicalDevice* m_PhysicalDevice;

        VkDevice m_Device;

        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;

    };
}

