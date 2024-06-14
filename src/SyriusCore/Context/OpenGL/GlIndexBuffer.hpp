#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/IndexBuffer.hpp"

namespace Syrius{

    class GlIndexBuffer : public IndexBuffer{
    public:
        GlIndexBuffer(const IndexBufferDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        ~GlIndexBuffer() override;

        void bind() override;

        void setData(const void* data, uint32 count) override;

        void copyFrom(const ResourceView<IndexBuffer>& other) override;

        [[nodiscard]] Resource<ubyte[]> getData() const override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_BufferID;
    };
}

