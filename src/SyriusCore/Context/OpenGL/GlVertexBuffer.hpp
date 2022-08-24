#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/VertexBuffer.hpp"

namespace Syrius{

    class GlVertexBuffer : public VertexBuffer{
    public:
        explicit GlVertexBuffer(const VertexBufferDesc& desc);

        ~GlVertexBuffer() override;

        void bind() override;

        void unbind() override;

    private:
        uint32 m_BufferID;

    };
}

