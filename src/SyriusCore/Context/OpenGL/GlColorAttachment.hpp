#pragma once

#include "../../../../include/SyriusCore/Context/ColorAttachment.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlColorAttachment: public ColorAttachment{
    public:
        explicit GlColorAttachment(const ColorAttachmentDesc& desc);

        ~GlColorAttachment() override;

        void bind(uint32 slot) override;

        [[nodiscard]] Image* getData() override;

        void onResize(uint32 width, uint32 height) override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        GLint m_InternalFormat;
        GLenum m_GlFormat;

        uint32 m_TextureID;
        uint32 m_ChannelCount;

    };

}

