#pragma once

#include "../../../../include/SyriusCore/Context/DepthStencilAttachment.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlDepthStencilAttachment : public DepthStencilAttachment{
    public:
        explicit GlDepthStencilAttachment(const DepthStencilAttachmentDesc& desc, uint32 framebufferID);

        ~GlDepthStencilAttachment() override;

        void bind() override;

        void unbind() override;

        void bindShaderResource(uint32 slot) override;

        void clear() override;

        void setSize(uint32 width, uint32 height) override;

        void enableDepthTest(bool enable) override;

        [[nodiscard]] Resource<Image> getData() override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_BufferID;
        uint32 m_FrameBufferID;
        GLenum m_GlFormat;

        GLenum m_GlDepthFunc;

        GLenum m_GlStencilFunc;
        GLenum m_GlStencilFail;
        GLenum m_GlStencilPassDepthFail;
        GLenum m_GlStencilPass;

    };

    class GlDefaultDepthStencilAttachment : public DepthStencilAttachment{
    public:
        explicit GlDefaultDepthStencilAttachment(const DepthStencilAttachmentDesc& desc);

        ~GlDefaultDepthStencilAttachment() override;

        void bind() override;

        void unbind() override;

        void bindShaderResource(uint32 slot) override;

        void clear() override;

        void setSize(uint32 width, uint32 height) override;

        void enableDepthTest(bool enable) override;

        [[nodiscard]] Resource<Image> getData() override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_FrameBufferID;

        GLenum m_GlDepthFunc;

        GLenum m_GlStencilFunc;
        GLenum m_GlStencilFail;
        GLenum m_GlStencilPassDepthFail;
        GLenum m_GlStencilPass;

    };
}