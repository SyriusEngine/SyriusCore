#pragma once

#include <optional>
#include <set>

#include "../../../../include/SyriusCore/Context/Context.hpp"
#include "../../Core/CoreCommand.hpp"
#include "VulkanUtils.hpp"
#include "VulkanLogicalDevice.hpp"
#include "VulkanPhysicalDevice.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanShader.hpp"
#include "VulkanShaderModule.hpp"

namespace Syrius{

    class VulkanContext: public Context{
    public:

        explicit VulkanContext(const ContextDesc& desc);

        ~VulkanContext() override;

        std::string getAPIVersion() override;

        std::string getDeviceName() override;

        std::string getShadingLanguageVersion() override;

        int32 getMaxFramebufferWidth() override;

        int32 getMaxFramebufferHeight() override;

        int32 getMaxFramebufferTextureAttachments() override;

        int32 getMaxTextureSlots() override;

        int32 getMaxTexture2DSize() override;

        int32 getMaxConstantBufferSize() override;

        int32 getMaxDepthBufferBits() override;

        ShaderModule* createShaderModule(const ShaderModuleDesc& desc) override;

        Shader* createShader(const ShaderDesc& desc) override;

        VertexBuffer* createVertexBuffer(const VertexBufferDesc& desc) override;

        IndexBuffer* createIndexBuffer(const IndexBufferDesc& desc) override;

        VertexArray* createVertexArray(const VertexArrayDesc& desc) override;

        ConstantBuffer* createConstantBuffer(const ConstantBufferDesc& desc) override;

        FrameBuffer* createFrameBuffer(const FrameBufferDesc& desc) override;

        Texture2D* createTexture2D(const Texture2DDesc& desc) override;

    protected:

        void createDevicesAndSwapChain();

    protected:
        VkInstance m_Instance;
        VkSurfaceKHR m_Surface;

        VulkanPhysicalDevice* m_PhysicalDevice;
        VulkanLogicalDevice* m_Device;
        VulkanSwapChain* m_SwapChain;
    };

}

