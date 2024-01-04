#pragma once

#include "../../../../include/SyriusCore/Context/Context.hpp"
#include "GlShaderModule.hpp"
#include "GlShader.hpp"
#include "GlViewport.hpp"
#include "GlVertexBuffer.hpp"
#include "GlIndexBuffer.hpp"
#include "GlVertexArray.hpp"
#include "GlTexture2D.hpp"
#include "GlConstantBuffer.hpp"
#include "GlSampler.hpp"
#include "GlFrameBuffer.hpp"
#include "GlCubemap.hpp"


namespace Syrius{

    class GlContext: public Context{
    public:

        ~GlContext() override;

        virtual void makeCurrent() = 0;

        inline void ensureCurrentContext(){
            if (m_ActiveContextID != m_ID){
                makeCurrent();
                m_ActiveContextID = m_ID;
            }
        }

        std::string getAPIVersion() override;

        std::string getDeviceName() override;

        std::string getDeviceVendor() override;

        std::string getShadingLanguageVersion() override;

        int32 getMaxFramebufferWidth() override;

        int32 getMaxFramebufferHeight() override;

        int32 getMaxFramebufferTextureAttachments() override;

        int32 getMaxTextureSlots() override;

        int32 getMaxTexture2DSize() override;

        int32 getMaxConstantBufferSize() override;

        int32 getMaxDepthBufferBits() override;

        ResourceView<ShaderModule> createShaderModule(const ShaderModuleDesc& desc) override;

        ResourceView<Shader> createShader(const ShaderDesc& desc) override;

        ResourceView<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc) override;

        ResourceView<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc) override;

        ResourceView<VertexArray> createVertexArray(const VertexArrayDesc& desc) override;

        ResourceView<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc& desc) override;

        ResourceView<Texture2D> createTexture2D(const Texture2DDesc& desc) override;

        ResourceView<Texture2D> createTexture2D(const Texture2DImageDesc& desc) override;

        ResourceView<Sampler> createSampler(const SamplerDesc& desc) override;

        ResourceView<FrameBuffer> createFrameBuffer(const ResourceView<FrameBufferDescription>& desc) override;

        ResourceView<Cubemap> createCubemap(const CubemapDesc& desc) override;

        ResourceView<Cubemap> createCubemap(const CubemapImageDesc& desc) override;

    protected:
        explicit GlContext(const ContextDesc& desc);
        
        void initGl(const ContextDesc& desc);

        void terminateGl();

        void initGlad();

        void terminateGlad();

    private:
        static uint32 m_ContextCount;
        static uint64 m_ActiveContextID;

        uint64 m_ID;

    };

}
