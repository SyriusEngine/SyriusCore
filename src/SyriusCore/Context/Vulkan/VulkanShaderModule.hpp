#pragma once

#include "VulkanUtils.hpp"
#include "VulkanLogicalDevice.hpp"
#include "../../../../include/SyriusCore/Context/ShaderModule.hpp"

namespace Syrius{

    class VulkanShaderModule : public ShaderModule{
    public:
        VulkanShaderModule(const ShaderModuleDesc& desc, VulkanLogicalDevice* device);

        ~VulkanShaderModule() override;

    private:

        void loadSPIRV(const ShaderModuleDesc& desc);

        void loadGlsl(const ShaderModuleDesc& desc);

    private:
        VulkanLogicalDevice* m_Device;

        VkShaderModule m_Module;


    };
}

