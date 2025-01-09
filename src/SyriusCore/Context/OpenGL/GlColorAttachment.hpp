#pragma once

#include "../../../../include/SyriusCore/Context/ColorAttachment.hpp"
#include "GlUtils.hpp"

namespace Syrius{
    
    class GlColorAttachment : public ColorAttachment{
    public:
        GlColorAttachment(const ColorAttachmentDesc& desc, const UP<DeviceLimits>& deviceLimits, u32 framebufferID, u32 attachmentID);
        
        ~GlColorAttachment() override;

        void bind() override;

        void bindShaderResource(u32 slot) override;

        void clear() override;

        void onResize(u32 width, u32 height) override;

        [[nodiscard]] UP<Image> getData() override;

        [[nodiscard]] u64 getIdentifier() const override;

    private:
        u32 m_TextureID;
        const u32 m_FrameBufferID;
        const u32 m_AttachmentID;

        GLenum m_GlChannelFormat;
        GLint m_GlInternalFormat;
        GLenum m_GlDataType;
    };

    class GlDefaultColorAttachment : public ColorAttachment{
    public:
        GlDefaultColorAttachment(const ColorAttachmentDesc& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlDefaultColorAttachment() override;

        void bind() override;

        void bindShaderResource(u32 slot) override;

        void clear() override;

        void onResize(u32 width, u32 height) override;

        [[nodiscard]] UP<Image> getData() override;

        [[nodiscard]] u64 getIdentifier() const override;

    };
}
