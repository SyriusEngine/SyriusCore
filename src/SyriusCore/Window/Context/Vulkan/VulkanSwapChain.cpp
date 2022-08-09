#include "VulkanSwapChain.hpp"

namespace Syrius{

    VulkanSwapChain::VulkanSwapChain(VulkanPhysicalDevice* physicalDevice, VulkanLogicalDevice* device, VkSurfaceKHR surface, const FramebufferSize& framebufferSize)
    : m_PhysicalDevice(physicalDevice),
    m_Device(device){
       SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_PhysicalDevice->getPhysicalDevice(), surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.m_Formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.m_PresentModes);
        m_Extent = chooseSwapExtent(swapChainSupport.m_Capabilities, framebufferSize);

        uint32_t imageCount = swapChainSupport.m_Capabilities.minImageCount + 1; // more performant to request at least 1 extra image
        if(swapChainSupport.m_Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.m_Capabilities.maxImageCount){
            imageCount = swapChainSupport.m_Capabilities.maxImageCount;
        }

        QueueFamilyIndices indices = m_PhysicalDevice->getQueueFamilyIndices();
        uint32_t queueFamilyIndices[] = {indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value()};

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = m_Extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        if (indices.m_GraphicsFamily != indices.m_PresentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }
        createInfo.preTransform = swapChainSupport.m_Capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;      // used for transparency
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        SR_VULKAN_CALL(vkCreateSwapchainKHR(m_Device->getDevice(), &createInfo, nullptr, &m_SwapChain), "Failed to create swap chain!");

        m_ImageFormat = surfaceFormat.format;

        SR_VULKAN_CALL(vkGetSwapchainImagesKHR(m_Device->getDevice(), m_SwapChain, &imageCount, nullptr), "Failed to get swap chain images!");
        m_Images.resize(imageCount);
        SR_VULKAN_CALL(vkGetSwapchainImagesKHR(m_Device->getDevice(), m_SwapChain, &imageCount, m_Images.data()), "Failed to get swap chain images!");


    }

    VulkanSwapChain::~VulkanSwapChain() {
        if (m_SwapChain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(m_Device->getDevice(), m_SwapChain, nullptr);
        }

    }

    VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }
        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, const FramebufferSize& framebufferSize) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32>::max()) {
            return capabilities.currentExtent;
        }
        else {
            VkExtent2D extent ={
                    std::clamp(framebufferSize.m_Width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                    std::clamp(framebufferSize.m_Height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
            };
            return extent;
        }
    }
}