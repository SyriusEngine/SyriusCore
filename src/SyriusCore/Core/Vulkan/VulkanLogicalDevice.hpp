#pragma once

#include "VulkanQueueFamily.hpp"
#include "VulkanPhysicalDevice.hpp"
#include "VulkanInstance.hpp"

namespace Syrius{

    class VulkanLogicalDevice{
    public:
        VulkanLogicalDevice(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice);

        ~VulkanLogicalDevice();

        VkDevice getDevice();

    private:
        VulkanInstance* m_Instance;
        VulkanPhysicalDevice* m_PhysicalDevice;

        VkDevice m_Device;
        VkQueue m_GraphicsQueue;
    };

}

