#include "VulkanShaderModule.hpp"

namespace Syrius{

    VulkanShaderModule::VulkanShaderModule(const ShaderModuleDesc &desc, VulkanLogicalDevice* device)
    : ShaderModule(desc),
    m_Module(VK_NULL_HANDLE),
    m_Device(device){
        SR_CORE_PRECONDITION(desc.m_CodeType != SR_SHADER_CODE_HLSL, "HLSL is not supported in Vulkan");

        if (desc.m_CodeType == SR_SHADER_CODE_SPIRV){
            loadSPIRV(desc);
        }
        else{
            loadGlsl(desc);
        }
    }

    VulkanShaderModule::~VulkanShaderModule() {
        if (m_Module != VK_NULL_HANDLE){
            vkDestroyShaderModule(m_Device->getDevice(), m_Module, nullptr);
        }
    }

    uint64 VulkanShaderModule::getIdentifier() const {
        return reinterpret_cast<uint64>(m_Module);
    }

    void VulkanShaderModule::loadSPIRV(const ShaderModuleDesc &desc) {
        const char* code = nullptr;
        if (desc.m_LoadType == SR_LOAD_FROM_FILE){
            code = readFileBinary(desc.m_Code).c_str();
        }
        else{
            code = desc.m_Code.c_str();
        }

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = desc.m_CodeLength;
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code);

        SR_VULKAN_CALL(vkCreateShaderModule(m_Device->getDevice(), &createInfo, nullptr, &m_Module), "Failed to create shader module");
    }

    void VulkanShaderModule::loadGlsl(const ShaderModuleDesc &desc) {

    }

}
