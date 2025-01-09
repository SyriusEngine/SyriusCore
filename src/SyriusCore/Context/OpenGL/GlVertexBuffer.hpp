#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/VertexBuffer.hpp"

namespace Syrius{

    class GlVertexBuffer : public VertexBuffer{
    public:
        GlVertexBuffer(const VertexBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlVertexBuffer() override;

        void bind() override;

        void setData(const void* data, u32 count) override;

        void copyFrom(const ResourceView<VertexBuffer>& other) override;

        [[nodiscard]] UP<UByte[]> getData() const override;

        [[nodiscard]] u64 getIdentifier() const override;

    private:
        u32 m_BufferID;

    };
}

