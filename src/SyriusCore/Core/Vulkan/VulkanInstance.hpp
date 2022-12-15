#pragma once

#include "../PlatformInclude.hpp"
#include "../DebugMacros.hpp"
#include "../../../../include/SyriusCore/Core/PlatformAPI.hpp"

namespace Syrius{

    class SR_CORE_API VulkanInstance{
    public:
        VulkanInstance() = delete;

        explicit VulkanInstance(const PlatformAPI* platformAPI);

        ~VulkanInstance();

        [[nodiscard]] std::vector<std::string> getSupportedExtensions() const;

        [[nodiscard]] std::vector<std::string> getSupportedLayers() const;

        VkInstance& getInstance();

    private:

        void setupDebugMessenger();

        VkResult createDebugUtilsMessenger(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

        void destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

    private:
        const PlatformAPI* m_PlatformAPI;
        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;



    };
}

