#pragma once

#include "../../../../include/SyriusCore/Context/Context.hpp"
#include "../../Core/CoreCommand.hpp"
#include "GlShaderModule.hpp"
#include "GlShader.hpp"
#include "GlVertexBuffer.hpp"
#include "GlIndexBuffer.hpp"
#include "GlVertexArray.hpp"
#include "GlTexture2D.hpp"
#include "GlConstantBuffer.hpp"
#include "GlFrameBuffer.hpp"
#include "GlSampler.hpp"

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

        ShaderModule* createShaderModule(const ShaderModuleDesc& desc) override;

        Shader* createShader(const ShaderDesc& desc) override;

        VertexBuffer* createVertexBuffer(const VertexBufferDesc& desc) override;

        IndexBuffer* createIndexBuffer(const IndexBufferDesc& desc) override;

        VertexArray* createVertexArray(const VertexArrayDesc& desc) override;

        ConstantBuffer* createConstantBuffer(const ConstantBufferDesc& desc) override;

        FrameBuffer* createFrameBuffer(const FrameBufferDesc& desc) override;

        Texture2D* createTexture2D(const Texture2DDesc& desc) override;

        Sampler* createSampler(const SamplerDesc& desc) override;

    protected:
        explicit GlContext(const ContextDesc& desc);
        
        void initGl(const FrameBufferDesc& desc);

    private:

        static uint64 m_ActiveContextID;

        uint64 m_ID;

    };

}
