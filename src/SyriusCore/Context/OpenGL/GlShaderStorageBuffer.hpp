#pragma once

#include "../../../../include/SyriusCore/Context/ShaderStorageBuffer.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlShaderStorageBuffer : public ShaderStorageBuffer{
    public:
        GlShaderStorageBuffer(const ShaderStorageBufferDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        ~GlShaderStorageBuffer() override;

        void bind() override;

        void bindShaderResource(uint32 slot) override;

        void setData(const void* data, uint32 size) override;

        [[nodiscard]] Resource<ubyte[]> getData() const override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_BufferID;

    };
}