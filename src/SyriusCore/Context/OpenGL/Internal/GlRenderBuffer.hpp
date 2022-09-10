#pragma once

#include "../../../../../include/SyriusCore/Utils/Image.hpp"
#include "../GlUtils.hpp"

namespace Syrius{

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

