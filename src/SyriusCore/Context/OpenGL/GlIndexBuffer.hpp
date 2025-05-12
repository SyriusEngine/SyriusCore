#pragma once

#include "../../../../include/SyriusCore/Context/IndexBuffer.hpp"
#include "Internal/GlBuffer.hpp"

namespace Syrius{

    class GlIndexBuffer : public IndexBuffer{
    public:
        GlIndexBuffer(const IndexBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlIndexBuffer() override = default;

        void release() override;

        void bind() override;

        void setData(const void* data, u64 count) override;

        void copyFrom(const ResourceView<IndexBuffer>& other) override;

        [[nodiscard]] UP<UByte[]> getData() const override;

        [[nodiscard]] u64 getIdentifier() const override;

        [[nodiscard]] Size getSize() const override;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const override;

    private:
        GlBuffer m_Buffer;
    };
}

