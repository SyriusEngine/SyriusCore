#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/IndexBuffer.hpp"

namespace Syrius{

    class GlIndexBuffer : public IndexBuffer{
    public:
        explicit GlIndexBuffer(const IndexBufferDesc& desc);

        ~GlIndexBuffer() override;

        void bind() override;

        void setData(const void* data) override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_BufferID;
    };
}

