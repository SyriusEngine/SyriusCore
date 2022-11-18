#pragma once

#include "../Core/SyriusCoreInclude.hpp"
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

    struct SR_API ContextDesc{
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

    struct SR_API FramebufferSize{
        uint32 m_Width  = 0;
        uint32 m_Height = 0;
    };

    class SR_API Context{
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
            m_DefaultFrameBuffer->setClearColor(r, g, b, a);
        }

        void setClearColor(float r, float g, float b, float a, FrameBuffer *frameBuffer);

        inline void clear(){
            clear(m_DefaultFrameBuffer);
        }

        void clear(FrameBuffer* frameBuffer);

        void draw(VertexArray* vao);

        void drawInstanced(VertexArray* vao, uint32 instanceCount);

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

        void destroyShaderModule(ShaderModule* shaderModule);

        void destroyShader(Shader* shader);

        void destroyVertexDescription(VertexDescription* vertexDescription);

        void destroyVertexBuffer(VertexBuffer* vertexBuffer);

        void destroyIndexBuffer(IndexBuffer* indexBuffer);

        void destroyVertexArray(VertexArray* vertexArray);

        void destroyConstantBuffer(ConstantBuffer* constantBuffer);

        void destroyFrameBuffer(FrameBuffer* frameBuffer);

        void destroyTexture2D(Texture2D* texture2D);

        void destroySampler(Sampler* sampler);

        virtual ShaderModule* createShaderModule(const ShaderModuleDesc& desc) = 0;

        virtual Shader* createShader(const ShaderDesc& desc) = 0;

        VertexDescription* createVertexDescription();

        virtual VertexBuffer* createVertexBuffer(const VertexBufferDesc& desc) = 0;

        virtual IndexBuffer* createIndexBuffer(const IndexBufferDesc& desc) = 0;

        virtual VertexArray* createVertexArray(const VertexArrayDesc& desc) = 0;

        virtual ConstantBuffer* createConstantBuffer(const ConstantBufferDesc& desc) = 0;

        virtual FrameBuffer* createFrameBuffer(const FrameBufferDesc& desc) = 0;

        virtual Texture2D* createTexture2D(const Texture2DDesc& desc) = 0;

        virtual Sampler* createSampler(const SamplerDesc& desc) = 0;

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

        std::vector<ShaderModule*> m_ShaderModules;
        std::vector<Shader*> m_Shaders;
        std::vector<VertexDescription*> m_VertexDescriptions;
        std::vector<VertexBuffer*> m_VertexBuffers;
        std::vector<IndexBuffer*> m_IndexBuffers;
        std::vector<VertexArray*> m_VertexArrays;
        std::vector<ConstantBuffer*> m_ConstantBuffers;
        std::vector<FrameBuffer*> m_FrameBuffers;
        std::vector<Texture2D*> m_Textures2D;
        std::vector<Sampler*> m_Samplers;


    private:

        SR_SUPPORTED_API m_Type;

    };

}