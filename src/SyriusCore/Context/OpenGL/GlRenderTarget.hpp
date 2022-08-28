#pragma once

#include "../../../../include/SyriusCore/Utils/Image.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlColorAttachment{
    public:
        GlColorAttachment(uint32 width, uint32 height, SR_TEXTURE_DATA_FORMAT format);

        ~GlColorAttachment();

        void bind(uint32 slot) const;

        [[nodiscard]] Image* readColorAttachment() const;

        void onResize(uint32 width, uint32 height);

        [[nodiscard]] uint32 getIdentifier() const;

    private:
        uint32 m_Width;
        uint32 m_Height;
        uint32 m_ChannelCount;
        GLint m_InternalFormat;
        GLenum m_Format;

        uint32 m_TextureID;
    };

    class GlRenderBuffer{
    public:
        GlRenderBuffer(uint32 width, uint32 height);

        ~GlRenderBuffer();

        void bind() const;

        void onResize(uint32 width, uint32 height);

        [[nodiscard]] uint32 getIdentifier() const;

    private:
        uint32 m_Width;
        uint32 m_Height;
        uint32 m_BufferID;
        GLenum m_InternalFormat;
    };
}

