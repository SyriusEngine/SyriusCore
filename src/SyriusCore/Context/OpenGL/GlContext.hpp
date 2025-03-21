#pragma once

#include "../../../../include/SyriusCore/Context/Context.hpp"
#include "GlDeviceLimits.hpp"
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
#include "GlCubeMap.hpp"


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

        ResourceView<ShaderModule> createShaderModule(const ShaderModuleDesc& desc) override;

        ResourceView<Shader> createShader(const ShaderDesc& desc) override;

        ResourceView<ShaderModule> createShaderModule(const ShaderModuleFileDesc& desc) override;

        ResourceView<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc) override;

        ResourceView<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc) override;

        ResourceView<VertexArray> createVertexArray(const VertexArrayDesc& desc) override;

        ResourceView<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc& desc) override;

        ResourceView<Texture2D> createTexture2D(const Texture2DDesc& desc) override;

        ResourceView<Texture2D> createTexture2D(const Texture2DImageDesc& desc) override;

        ResourceView<Sampler> createSampler(const SamplerDesc& desc) override;

        ResourceView<FrameBuffer> createFrameBuffer(const ResourceView<FrameBufferLayout>& desc) override;

        ResourceView<CubeMap> createCubeMap(const ResourceView<CubeMapLayout>& desc) override;

    protected:
        explicit GlContext(const ContextDesc& desc);
        
        void initGl();

        void createDefaultFrameBuffer(i32 width, i32 height, const ContextDesc& desc) override;

        void terminateGl();

        void initGlad();

        void terminateGlad();

    private:
        static u32 m_ContextCount;
        static u64 m_ActiveContextID;

        u64 m_ID;

    };

}
