#pragma once

#include "VulkanUtils.hpp"
#include "VulkanPhysicalDevice.hpp"
#include "VulkanLogicalDevice.hpp"
#include "../../../../../include/SyriusCore/Window/Context/Context.hpp"

namespace Syrius{

    class VulkanSwapChain{
    public:
        VulkanSwapChain(VulkanPhysicalDevice* physicalDevice, VulkanLogicalDevice* device, VkSurfaceKHR surface, const FramebufferSize& framebufferSize);

        ~VulkanSwapChain();

    private:

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const FramebufferSize& framebufferSize);

    private:
        VulkanPhysicalDevice* m_PhysicalDevice;
        VulkanLogicalDevice* m_Device;

        VkSwapchainKHR m_SwapChain;
        VkFormat m_ImageFormat;
        VkExtent2D m_Extent;
        std::vector<VkImage> m_Images;
    };
}

