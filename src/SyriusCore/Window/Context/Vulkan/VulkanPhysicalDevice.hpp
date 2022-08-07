#pragma once

#include "VulkanUtils.hpp"

namespace Syrius{

    class VulkanPhysicalDevice{
    public:
        VulkanPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

        ~VulkanPhysicalDevice();

        [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const;

        [[nodiscard]] VkPhysicalDeviceProperties getDeviceProperties() const;

        [[nodiscard]] VkPhysicalDeviceLimits getDeviceLimits() const;

    private:

        uint32 ratePhysicalDevice(VkPhysicalDevice device);

    private:
        VkInstance m_Instance;
        VkSurfaceKHR m_Surface;
        VkPhysicalDevice m_Device;

    };

}

