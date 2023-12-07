#pragma once

#include "../Utils/SyriusCoreInclude.hpp"
#include "../Utils/Resource.hpp"
#include "ShaderModule.hpp"
#include "Shader.hpp"
#include "Viewport.hpp"
#include "VertexLayout.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture2D.hpp"
#include "ConstantBuffer.hpp"
#include "Sampler.hpp"
#include "FramebufferDescription.hpp"
#include "Framebuffer.hpp"
#include "ColorAttachment.hpp"
#include "DepthStencilAttachment.hpp"

namespace Syrius{

    class Window;

    struct SR_CORE_API ContextDesc{
        uint8 depthBits        = 24;
        uint8 stencilBits      = 8;
        uint8 redBits          = 8;
        uint8 blueBits         = 8;
        uint8 greenBits        = 8;
        uint8 alphaBits        = 8;
        uint32 backBufferWidth = 0; // if 0, will be set to window width
        uint32 backBufferHeight= 0; // if 0, will be set to window height
        bool enableDepthTest   = false;
        bool enableStencilTest = false;
        SR_SUPPORTED_API api   = SR_API_OPENGL;
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

        ResourceView<FrameBuffer> getDefaultFrameBuffer();

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

        virtual ResourceView<ShaderModule> createShaderModule(const ShaderModuleDesc& desc) = 0;

        virtual ResourceView<Shader> createShader(const ShaderDesc& desc) = 0;

        ResourceView<VertexLayout> createVertexDescription();

        virtual ResourceView<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc) = 0;

        virtual ResourceView<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc) = 0;

        virtual ResourceView<VertexArray> createVertexArray(const VertexArrayDesc& desc) = 0;

        virtual ResourceView<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc& desc) = 0;

        virtual ResourceView<Texture2D> createTexture2D(const Texture2DDesc& desc) = 0;

        virtual ResourceView<Texture2D> createTexture2D(const Texture2DImageDesc& desc) = 0;

        virtual ResourceView<Sampler> createSampler(const SamplerDesc& desc) = 0;

        ResourceView<FrameBufferDescription> createFrameBufferDescription();

        virtual ResourceView<FrameBuffer> createFrameBuffer(const ResourceView<FrameBufferDescription>& desc) = 0;

        void onResize(uint32 width, uint32 height);

        virtual void beginRenderPass(const ResourceView<FrameBuffer>& frameBuffer);

        virtual void beginRenderPass();

        virtual void endRenderPass(const ResourceView<FrameBuffer>& frameBuffer);

        virtual void endRenderPass();

        /**
         * @brief Clears the default framebuffer
         */
        inline void clear(){
            m_FrameBuffers[0]->clear();
        }

        /**
         * @brief Sets the clear color of the default framebuffer
         * @param r red component
         * @param g green component
         * @param b blue component
         * @param a alpha component
         */
        inline void setClearColor(float r, float g, float b, float a = 1.0f){
            m_FrameBuffers[0]->getColorAttachment(0)->setClearColor(r, g, b, a);
        }

        /**
         * @brief returns the clear color of the default framebuffer. This default framebuffer only has one color attachment.
         *        The returned array is of size 4 and contains the components in the following order: r, g, b, a
         * @return float array containing the clear color
         */
        inline float* getClearColor(){
            return m_FrameBuffers[0]->getColorAttachment(0)->getClearColor();
        }

        inline void draw(const ResourceView<VertexArray>& vertexArray){
            vertexArray->drawBuffers();
        }

        inline void drawInstanced(const ResourceView<VertexArray>& vertexArray, uint32 instanceCount){
            vertexArray->drawBuffersInstanced(instanceCount);
        }

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

    protected:

        explicit Context(const ContextDesc& desc);

        virtual void createImGuiContext() = 0;

        virtual void destroyImGuiContext() = 0;

        virtual void onImGuiBegin() = 0;

        virtual void onImGuiEnd() = 0;

    protected:

        friend Window;

    protected:
        uint32 m_Width;
        uint32 m_Height;

        bool m_VerticalSync;

        /*
         * The reason why the context must own all resources at all times is that the objects always depend on some
         * resources that the context manages. For example, D3D11 owns the device, device context and swap chain. If
         * the context is destroyed, the device, device context and swap chain are destroyed as well. If not all objects
         * are destroyed BEFORE this happens, the application will crash. Same for OpenGL but here the problem lies
         * with gl function pointer loading. If the context is destroyed, the function pointers are no longer valid.
         * This problem is the main reason why the context must own all resources and why we wrote our own smart pointer
         * that can create 'Views' of actual resources. This way, the context can destroy all resources when it is destroyed
         * and the corresponding views will be invalidated. This way, no dangling pointers will be left behind.
         */
        std::vector<Resource<ShaderModule>> m_ShaderModules;
        std::vector<Resource<Shader>> m_Shaders;
        std::vector<Resource<VertexLayout>> m_VertexDescriptions;
        std::vector<Resource<VertexBuffer>> m_VertexBuffers;
        std::vector<Resource<IndexBuffer>> m_IndexBuffers;
        std::vector<Resource<VertexArray>> m_VertexArrays;
        std::vector<Resource<ConstantBuffer>> m_ConstantBuffers;
        std::vector<Resource<Texture2D>> m_Textures2D;
        std::vector<Resource<Sampler>> m_Samplers;
        std::vector<Resource<FrameBufferDescription>> m_FrameBufferDescriptions;
        std::vector<Resource<FrameBuffer>> m_FrameBuffers; // framebuffer at location 0 is the default framebuffer


    private:

        SR_SUPPORTED_API m_Type;

    };

}