#pragma once

#include "../../../../include/SyriusCore/Context/Context.hpp"
#include "../../Core/CoreCommand.hpp"
#include "GlShaderModule.hpp"
#include "GlShader.hpp"
#include "GlVertexBuffer.hpp"
#include "GlIndexBuffer.hpp"
#include "GlVertexArray.hpp"

namespace Syrius{

    class GlContext: public Context{
    public:

        ~GlContext() override;

        virtual void makeCurrent() = 0;

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

        Shader* createShader() override;

        VertexBuffer* createVertexBuffer(const VertexBufferDesc& desc) override;

        IndexBuffer* createIndexBuffer(const IndexBufferDesc& desc) override;

        VertexArray* createVertexArray(const VertexArrayDesc& desc) override;

    protected:
        GlContext();
        
        void initGl();
    };

}
