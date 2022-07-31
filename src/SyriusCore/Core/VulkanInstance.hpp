#pragma once

#include "PlatformInclude.hpp"
#include "DebugMacros.hpp"
#include "../../../../include/SyriusCore/Core/PlatformAPI.hpp"

namespace Syrius{

    class SR_API VulkanInstance{
    public:
        VulkanInstance() = delete;

        explicit VulkanInstance(const PlatformAPI* platformAPI);

        ~VulkanInstance();

        [[nodiscard]] std::vector<std::string> getSupportedExtensions() const;

        [[nodiscard]] std::vector<std::string> getSupportedLayers() const;

    private:

        void setupDebugMessenger();

    private:
        const PlatformAPI* m_PlatformAPI;
        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;



    };
}

