#pragma once

#include "../../../../include/SyriusCore/Context/DepthStencilAttachment.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlDepthStencilAttachment : public DepthStencilAttachment{
    public:
        GlDepthStencilAttachment(const DepthStencilAttachmentDesc& desc, const Resource<DeviceLimits>& deviceLimits, uint32 framebufferID);

        ~GlDepthStencilAttachment() override;

        void bind() override;

        void unbind() override;

        void clear() override;

        void enableDepthTest(bool enable) override;

        void setDepthFunc(SR_COMPARISON_FUNC func) override;

        void setDepthMask(SR_DEPTH_MASK mask) override;

        [[nodiscard]] Resource<Image> getData() override;

        [[nodiscard]] uint64 getIdentifier() const override;

    protected:
        uint32 m_BufferID;
        uint32 m_FrameBufferID;
        GLenum m_GlInternalFormat;

        GLenum m_GlDepthFunc;

        GLenum m_GlStencilFunc;
        GLenum m_GlStencilFail;
        GLenum m_GlStencilPassDepthFail;
        GLenum m_GlStencilPass;
    };

    class GlDepthStencilAttachmentRenderBuffer : public GlDepthStencilAttachment{
    public:
        GlDepthStencilAttachmentRenderBuffer(const DepthStencilAttachmentDesc& desc, const Resource<DeviceLimits>& deviceLimits, uint32 framebufferID);

        ~GlDepthStencilAttachmentRenderBuffer() override;

        void bind() override;

        void unbind() override;

        void bindShaderResource(uint32 slot) override;

        void onResize(uint32 width, uint32 height) override;
    };

    class GlDepthStencilAttachmentTexture : public GlDepthStencilAttachment{
    public:
        GlDepthStencilAttachmentTexture(const DepthStencilAttachmentDesc& desc, const Resource<DeviceLimits>& deviceLimits, uint32 framebufferID);

        ~GlDepthStencilAttachmentTexture() override;

        void bind() override;

        void unbind() override;

        void bindShaderResource(uint32 slot) override;

        void onResize(uint32 width, uint32 height) override;
    };

    class GlDefaultDepthStencilAttachment : public GlDepthStencilAttachment{
    public:
        GlDefaultDepthStencilAttachment(const DepthStencilAttachmentDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        ~GlDefaultDepthStencilAttachment() override;

        void bindShaderResource(uint32 slot) override;

        void onResize(uint32 width, uint32 height) override;

        [[nodiscard]] Resource<Image> getData() override;

        [[nodiscard]] uint64 getIdentifier() const override;
    };
}