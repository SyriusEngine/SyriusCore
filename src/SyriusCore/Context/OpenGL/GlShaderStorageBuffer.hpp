#pragma once

#include "../../../../include/SyriusCore/Context/ShaderStorageBuffer.hpp"
#include "Internal/GlBuffer.hpp"

namespace Syrius {

    class GlShaderStorageBuffer: public ShaderStorageBuffer{
    public:
        GlShaderStorageBuffer(const ShaderStorageBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlShaderStorageBuffer() override = default;

        void release() override;

        void bind() override;

        void bindShaderResource(u32 slot) override;

        void setData(const void* data, Size size) override;

        void copyFrom(const ResourceView<ShaderStorageBuffer>& other) override;

        [[nodiscard]] UP<UByte[]> getData() const override;

        [[nodiscard]] u64 getIdentifier() const override;

        [[nodiscard]] const std::string& getName() const override;

        [[nodiscard]] Size getSize() const override;

        [[nodiscard]] Size getCount() const override;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const override;

    private:
        GlBuffer m_Buffer;
    };
}