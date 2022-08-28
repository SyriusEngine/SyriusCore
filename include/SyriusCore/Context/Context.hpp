#pragma once

#include "../Core/SyriusCoreInclude.hpp"
#include "../../../src/SyriusCore/Core/DebugMacros.hpp"
#include "ShaderModule.hpp"
#include "Shader.hpp"
#include "VertexDescription.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture2D.hpp"
#include "ConstantBuffer.hpp"

namespace Syrius{

    struct FramebufferSize{
        uint32 m_Width;
        uint32 m_Height;
    };

    class SR_API Context{
    public:

        Context() = delete;

        Context(const Context&) = delete;

        virtual ~Context();

        virtual void swapBuffers() = 0;

        virtual void setVerticalSynchronisation(bool enable) = 0;

        virtual void createImGuiContext() = 0;

        virtual void destroyImGuiContext() = 0;

        virtual void onImGuiBegin() = 0;

        virtual void onImGuiEnd() = 0;

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

        virtual void setClearColor(float r, float g, float b, float a) = 0;

        virtual void clear() = 0;

        virtual void draw(VertexArray* vao) = 0;

        virtual void drawInstanced(VertexArray* vao, uint32 instanceCount) = 0;

        virtual ShaderModule* createShaderModule(const ShaderModuleDesc& desc) = 0;

        virtual Shader* createShader() = 0;

        VertexDescription* createVertexDescription();

        virtual VertexBuffer* createVertexBuffer(const VertexBufferDesc& desc) = 0;

        virtual IndexBuffer* createIndexBuffer(const IndexBufferDesc& desc) = 0;

        virtual VertexArray* createVertexArray(const VertexArrayDesc& desc) = 0;

        virtual ConstantBuffer* createConstantBuffer(const ConstantBufferDesc& desc) = 0;

        virtual Texture2D* createTexture2D() = 0;

    protected:

        explicit Context(SR_SUPPORTED_API type);

    protected:

        bool m_VerticalSync;

    private:

        SR_SUPPORTED_API m_Type;

    };

}