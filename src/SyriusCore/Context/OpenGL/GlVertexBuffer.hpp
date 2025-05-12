#pragma once

#include "../../../../include/SyriusCore/Context/VertexBuffer.hpp"
#include "Internal/GlBuffer.hpp"

namespace Syrius{

    class GlVertexBuffer : public VertexBuffer{
    public:
        GlVertexBuffer(const VertexBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlVertexBuffer() override = default;

        void release() override;

        void bind() override;

        void setData(const void* data, u64 count) override;

        void copyFrom(const ResourceView<VertexBuffer>& other) override;

        [[nodiscard]] UP<UByte[]> getData() const override;

        [[nodiscard]] u64 getIdentifier() const override;

        [[nodiscard]] Size getSize() const override;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const override;

    private:
        GlBuffer m_Buffer;

    };
}

