#pragma once

#include <vulkan/vulkan.hpp>
#include "PlatformInclude.hpp"
#include "../../../../include/SyriusCore/Core/PlatformAPI.hpp"

namespace Syrius{

    class SR_API VulkanInstance{
    public:
        VulkanInstance() = delete;

        explicit VulkanInstance(const PlatformAPI* platformAPI);

        ~VulkanInstance();

        [[nodiscard]] std::vector<std::string> getSupportedExtensions() const;

        [[nodiscard]] const VkInstance& getVkInstance() const;


    private:
        const PlatformAPI* m_PlatformAPI;
        VkInstance m_Instance;

    };
}

