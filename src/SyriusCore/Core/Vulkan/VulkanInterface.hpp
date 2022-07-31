#pragma once

#include "VulkanInstance.hpp"

namespace Syrius{

    class VulkanInterface{
    public:

        explicit VulkanInterface(const PlatformAPI* platformAPI);

        ~VulkanInterface();

        [[nodiscard]] std::vector<std::string> getSupportedExtensions() const;

        [[nodiscard]] std::vector<std::string> getSupportedLayers() const;

        [[nodiscard]] VulkanInstance* getInstance() const;

    private:
        VulkanInstance* m_Instance;
    };

}


