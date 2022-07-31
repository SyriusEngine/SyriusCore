#pragma once

#include "VulkanInstance.hpp"
#include "VulkanPhysicalDevice.hpp"
#include "VulkanLogicalDevice.hpp"

namespace Syrius{

    class VulkanInterface{
    public:

        explicit VulkanInterface(const PlatformAPI* platformAPI);

        ~VulkanInterface();

        [[nodiscard]] std::vector<std::string> getSupportedExtensions() const;

        [[nodiscard]] std::vector<std::string> getSupportedLayers() const;

        void createDevices();

    private:
        VulkanInstance* m_Instance;
        VulkanPhysicalDevice* m_PhysicalDevice;
        VulkanLogicalDevice* m_LogicalDevice;
    };

}


