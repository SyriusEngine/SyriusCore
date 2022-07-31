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

        [[nodiscard]] VulkanInstance* getInstance() const;

        [[nodiscard]] VulkanLogicalDevice* getLogicalDevice() const;

    private:
        VulkanInstance* m_Instance;
        VulkanPhysicalDevice* m_PhysicalDevice;
        VulkanLogicalDevice* m_LogicalDevice;
    };

}


