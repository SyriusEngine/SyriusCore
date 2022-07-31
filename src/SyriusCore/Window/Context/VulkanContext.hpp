#pragma once

#include "../../../../include/SyriusCore/Window/Context/Context.hpp"
#include "../../Core/CoreCommand.hpp"

namespace Syrius{

    class VulkanContext: public Context{
    public:

        VulkanContext();

        ~VulkanContext() override;

        VkSurfaceKHR& getSurface();

    protected:
        VkSurfaceKHR m_Surface;
        VkInstance m_Instance;
        VkDevice m_Device;
    };

}

