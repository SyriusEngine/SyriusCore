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

        [[nodiscard]] const std::vector<std::string>& getExtensionsNames() const;

        [[nodiscard]] QueueFamilyIndices getQueueFamilyIndices() const;

    private:

        uint32 ratePhysicalDevice(VkPhysicalDevice device);

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    private:
        std::vector<std::string> m_Extensions;

        VkInstance m_Instance;
        VkSurfaceKHR m_Surface;
        VkPhysicalDevice m_Device;

    };

}

