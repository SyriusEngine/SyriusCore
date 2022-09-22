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

namespace Syrius{

    class SyriusWindow;

    struct SR_API ContextDesc{
        FrameBufferDesc m_DefaultFrameBufferDesc    = FrameBufferDesc();
        uint32 m_BackBufferCount    = 1;
        uint32 m_DepthBits          = 24;
        uint32 m_StencilBits        = 8;
        uint32 m_RedBits            = 8;
        uint32 m_BlueBits           = 8;
        uint32 m_GreenBits          = 8;
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

        void setClearColor(float r, float g, float b, float a);

        void setClearColor(FrameBuffer* frameBuffer, float r, float g, float b, float a);

        void clear();

        void clear(FrameBuffer* frameBuffer);

        virtual void draw(VertexArray* vao) = 0;

        virtual void drawInstanced(VertexArray* vao, uint32 instanceCount) = 0;

        virtual void onResize(uint32 width, uint32 height) = 0;

        virtual ShaderModule* createShaderModule(const ShaderModuleDesc& desc) = 0;

        virtual Shader* createShader(const ShaderDesc& desc) = 0;

        VertexDescription* createVertexDescription();

        virtual VertexBuffer* createVertexBuffer(const VertexBufferDesc& desc) = 0;

        virtual IndexBuffer* createIndexBuffer(const IndexBufferDesc& desc) = 0;

        virtual VertexArray* createVertexArray(const VertexArrayDesc& desc) = 0;

        virtual ConstantBuffer* createConstantBuffer(const ConstantBufferDesc& desc) = 0;

        virtual FrameBuffer* createFrameBuffer(const FrameBufferDesc& desc) = 0;

        virtual Texture2D* createTexture2D(const Texture2DDesc& desc) = 0;

    protected:

        explicit Context(const ContextDesc& desc);

        virtual void createImGuiContext() = 0;

        virtual void destroyImGuiContext() = 0;

        virtual void onImGuiBegin() = 0;

        virtual void onImGuiEnd() = 0;

        friend SyriusWindow;

    protected:

        bool m_VerticalSync;
        FrameBuffer* m_DefaultFrameBuffer;

    private:

        SR_SUPPORTED_API m_Type;

    };

}