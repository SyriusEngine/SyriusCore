#pragma once

#include "VulkanInstance.hpp"
#include <optional>

namespace Syrius{

    struct QueueFamilyIndices{
        std::optional<uint32> m_GraphicsFamily;

        bool isComplete();
    };

    QueueFamilyIndices findQueueFamily(VkPhysicalDevice device);

}

