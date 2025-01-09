#pragma once

#include "../../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "GlUtils.hpp"
#include "GlShader.hpp"

namespace Syrius{

    class GlConstantBuffer: public ConstantBuffer{
    public:
        GlConstantBuffer(const ConstantBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlConstantBuffer() override;

        void bind(u32 slot) override;

        void setData(const void* data, u64 size) override;

        void copyFrom(const ResourceView<ConstantBuffer>& other) override;

        [[nodiscard]] UP<UByte[]> getData() const override;

        [[nodiscard]] u64 getIdentifier() const override;

    private:
        u32 m_BufferID;
    };
}

