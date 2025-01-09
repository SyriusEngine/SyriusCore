#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/IndexBuffer.hpp"

namespace Syrius{

    class GlIndexBuffer : public IndexBuffer{
    public:
        GlIndexBuffer(const IndexBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlIndexBuffer() override;

        void bind() override;

        void setData(const void* data, u32 count) override;

        void copyFrom(const ResourceView<IndexBuffer>& other) override;

        [[nodiscard]] UP<UByte[]> getData() const override;

        [[nodiscard]] u64 getIdentifier() const override;

    private:
        u32 m_BufferID;
    };
}

