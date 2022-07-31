#pragma once

#include <optional>
#include <set>

#include "../../../../include/SyriusCore/Window/Context/Context.hpp"
#include "../../Core/CoreCommand.hpp"

namespace Syrius{

    struct QueueFamilyIndices{
        std::optional<uint32> m_GraphicsFamily;
        std::optional<uint32> m_PresentFamily;

        bool isComplete();
    };

    class VulkanContext: public Context{
    public:

        VulkanContext();

        ~VulkanContext() override;

        VkSurfaceKHR& getSurface();

    protected:

        void selectPhysicalDevice();

        void createLogicalDevice();

    protected:
        VkInstance m_Instance;
        VkSurfaceKHR m_Surface;

    private:

        uint32 ratePhysicalDevice(VkPhysicalDevice device);

        QueueFamilyIndices findQueueFamily(VkPhysicalDevice device);

    private:
        VkPhysicalDevice m_PhysicalDevice;
        VkDevice m_Device;

        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;
    };

}

