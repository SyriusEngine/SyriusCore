#pragma once

#include "VulkanInstance.hpp"
#include "VulkanQueueFamily.hpp"

namespace Syrius{

    class VulkanPhysicalDevice{
    public:
        explicit VulkanPhysicalDevice(VkInstance instance);

        ~VulkanPhysicalDevice();

        VkPhysicalDevice getDevice();

    private:

       uint32 rateDevice(VkPhysicalDevice device);

    private:
        VkInstance& m_Instance;
        VkPhysicalDevice m_Device;
    };

}

