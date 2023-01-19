#pragma once

#include "../Core/SyriusCoreInclude.hpp"
#include "../Utils/Resource.hpp"
#include "ShaderModule.hpp"
#include "Shader.hpp"
#include "VertexDescription.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture2D.hpp"
#include "ConstantBuffer.hpp"
#include "FrameBuffer.hpp"
#include "ColorAttachment.hpp"
#include "Sampler.hpp"
#include "DepthStencilAttachment.hpp"

namespace Syrius{

    class SyriusWindow;

    struct SR_CORE_API ContextDesc{
        FrameBufferDesc m_DefaultFrameBufferDesc    = FrameBufferDesc();
        uint32 m_BackBufferCount    = 1;
        uint8 m_DepthBits          = 24;
        uint8 m_StencilBits        = 8;
        uint8 m_RedBits            = 8;
        uint8 m_BlueBits           = 8;
        uint8 m_GreenBits          = 8;
        uint8 m_AlphaBits          = 8;
        SR_SUPPORTED_API m_API      = SR_API_OPENGL;
    };

    struct SR_CORE_API FramebufferSize{
        uint32 m_Width  = 0;
        uint32 m_Height = 0;
    };

    class SR_CORE_API Context{
    public:

        Context() = delete;

        Context(const Context&) = delete;

        virtual ~Context();

        virtual void swapBuffers() = 0;

        virtual void setVerticalSynchronisation(bool enable) = 0;

        [[nodiscard]] SR_SUPPORTED_API getType() const;

        [[nodiscard]] bool isVerticalSyncEnabled() const;

        virtual std::string getAPIVersion() = 0;

        virtual std::string getDeviceName() = 0;

        virtual std::string getDeviceVendor() = 0;

        virtual std::string getShadingLanguageVersion() = 0;

        virtual int32 getMaxFramebufferWidth() = 0;

        virtual int32 getMaxFramebufferHeight() = 0;

        virtual int32 getMaxFramebufferTextureAttachments() = 0;

        virtual int32 getMaxTextureSlots() = 0;

        virtual int32 getMaxTexture2DSize() = 0;

        virtual int32 getMaxConstantBufferSize() = 0;

        virtual int32 getMaxDepthBufferBits() = 0;

        virtual FramebufferSize getFramebufferSize() = 0;

        FrameBuffer* getDefaultFrameBuffer();

        inline void setClearColor(float r, float g, float b, float a){
            setClearColor(r, g, b, a, m_DefaultFrameBuffer);
        }

        inline void setClearColor(float r, float g, float b, float a, FrameBuffer *frameBuffer){
            frameBuffer->getColorAttachment(0)->setClearColor(r, g, b, a);
        }

        inline void clear(){
            clear(m_DefaultFrameBuffer);
        }

        void clear(FrameBuffer* frameBuffer);

        void draw(ResourceView<VertexArray> vao);

        void drawInstanced(ResourceView<VertexArray> vao, uint32 instanceCount);

        inline void onResize(uint32 width, uint32 height){
            onResize(width, height, m_DefaultFrameBuffer);
        }

        void onResize(uint32 width, uint32 height, FrameBuffer* frameBuffer);

        void bindDefaultFrameBuffer();

        inline void beginRenderPass(){
            beginRenderPass(m_DefaultFrameBuffer);
        }

        void beginRenderPass(FrameBuffer* frameBuffer);

        inline void endRenderPass(){
            endRenderPass(m_DefaultFrameBuffer);
        }

        void endRenderPass(FrameBuffer* frameBuffer);

        virtual ResourceView<ShaderModule> createShaderModule(const ShaderModuleDesc& desc) = 0;

        virtual ResourceView<Shader> createShader(const ShaderDesc& desc) = 0;

        ResourceView<VertexDescription> createVertexDescription();

        virtual ResourceView<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc) = 0;

        virtual ResourceView<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc) = 0;

        virtual ResourceView<VertexArray> createVertexArray(const VertexArrayDesc& desc) = 0;

        virtual ResourceView<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc& desc) = 0;

        virtual ResourceView<FrameBuffer> createFrameBuffer(const FrameBufferDesc& desc) = 0;

        virtual ResourceView<Texture2D> createTexture2D(const Texture2DDesc& desc) = 0;

        virtual ResourceView<Texture2D> createTexture2D(const Texture2DImageDesc& desc) = 0;

        virtual ResourceView<Sampler> createSampler(const SamplerDesc& desc) = 0;

    protected:

        explicit Context(const ContextDesc& desc);

        virtual void createImGuiContext() = 0;

        virtual void destroyImGuiContext() = 0;

        virtual void onImGuiBegin() = 0;

        virtual void onImGuiEnd() = 0;

    protected:

        friend SyriusWindow;

    protected:

        bool m_VerticalSync;
        FrameBuffer* m_DefaultFrameBuffer;

        std::vector<Resource<ShaderModule>> m_ShaderModules;
        std::vector<Resource<Shader>> m_Shaders;
        std::vector<Resource<VertexDescription>> m_VertexDescriptions;
        std::vector<Resource<VertexBuffer>> m_VertexBuffers;
        std::vector<Resource<IndexBuffer>> m_IndexBuffers;
        std::vector<Resource<VertexArray>> m_VertexArrays;
        std::vector<Resource<ConstantBuffer>> m_ConstantBuffers;
        std::vector<Resource<FrameBuffer>> m_FrameBuffers;
        std::vector<Resource<Texture2D>> m_Textures2D;
        std::vector<Resource<Sampler>> m_Samplers;


    private:

        SR_SUPPORTED_API m_Type;

    };

}