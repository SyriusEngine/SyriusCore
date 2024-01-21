#pragma once

#include "../../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "GlUtils.hpp"
#include "GlShader.hpp"

namespace Syrius{

    class GlConstantBuffer: public ConstantBuffer{
    public:
        GlConstantBuffer(const ConstantBufferDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        ~GlConstantBuffer() override;

        void bind(uint32 slot) override;

        void setData(const void* data) override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_BufferID;
    };
}

