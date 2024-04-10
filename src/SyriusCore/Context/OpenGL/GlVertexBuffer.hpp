#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/VertexBuffer.hpp"

namespace Syrius{

    class GlVertexBuffer : public VertexBuffer{
    public:
        GlVertexBuffer(const VertexBufferDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        ~GlVertexBuffer() override;

        void bind() override;

        void setData(const void* data, uint32 count) override;

        [[nodiscard]] Resource<ubyte[]> getData() const override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:

        void createBuffer(const void* data);

    private:
        uint32 m_BufferID;

    };
}

