#pragma once

#include "../../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "Internal/GlBuffer.hpp"

namespace Syrius{

    class GlConstantBuffer: public ConstantBuffer{
    public:
        GlConstantBuffer(const ConstantBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlConstantBuffer() override = default;

        void release() override;

        void bind(u32 slot) override;

        void setData(const void* data, Size size) override;

        void copyFrom(const ResourceView<ConstantBuffer>& other) override;

        [[nodiscard]] UP<UByte[]> getData() const override;

        [[nodiscard]] u64 getIdentifier() const override;

        [[nodiscard]] const std::string& getName() const override;

        [[nodiscard]] Size getSize() const override;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const override;

    private:
        GlBuffer m_Buffer;
    };
}

