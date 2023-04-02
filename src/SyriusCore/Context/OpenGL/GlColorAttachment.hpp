#pragma once

#include "../../../../include/SyriusCore/Context/ColorAttachment.hpp"
#include "GlUtils.hpp"

namespace Syrius{
    
    class GlColorAttachment : public ColorAttachment{
    public:
        GlColorAttachment(uint32 framebufferID, uint32 attachmentID, const ColorAttachmentDesc& desc);
        
        ~GlColorAttachment() override;

        void bind() override;

        void bindShaderResource(uint32 slot) override;

        void clear() override;

        void setSize(uint32 width, uint32 height) override;

        [[nodiscard]] Resource<Image> getData() override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_TextureID;
        const uint32 m_FrameBufferID;
        const uint32 m_AttachmentID;

        GLint m_InternalFormat;
        GLenum m_GlFormat;
        GLenum m_GlDataType;
        uint32 m_ChannelCount;

    };

    class GlDefaultColorAttachment : public ColorAttachment{
    public:
        GlDefaultColorAttachment(const ColorAttachmentDesc& desc);

        ~GlDefaultColorAttachment() override;

        void bind() override;

        void bindShaderResource(uint32 slot) override;

        void clear() override;

        void setSize(uint32 width, uint32 height) override;

        [[nodiscard]] Resource<Image> getData() override;

        [[nodiscard]] uint64 getIdentifier() const override;

    };
}
