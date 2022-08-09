#pragma once

#include "../../../Core/PlatformInclude.hpp"
#include "../../../Core/DebugMacros.hpp"
#include "../../../Core/CoreCommand.hpp"
#include "../../../../../include/SyriusCore/Window/Context/Utils.hpp"

namespace Syrius{

    struct QueueFamilyIndices{
        std::optional<uint32> m_GraphicsFamily;
        std::optional<uint32> m_PresentFamily;

        bool isComplete();
    };

    QueueFamilyIndices findQueueFamily(VkPhysicalDevice device, VkSurfaceKHR surface);

    struct SwapChainSupportDetails{
        VkSurfaceCapabilitiesKHR m_Capabilities;
        std::vector<VkSurfaceFormatKHR> m_Formats;
        std::vector<VkPresentModeKHR> m_PresentModes;
    };

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

}