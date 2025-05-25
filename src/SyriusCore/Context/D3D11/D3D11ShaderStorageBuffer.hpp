#pragma once

#include "../../../../include/SyriusCore/Context/ShaderStorageBuffer.hpp"
#include "Internal/D3D11Buffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius {

    class D3D11ShaderStorageBuffer: public ShaderStorageBuffer{
    public:
        D3D11ShaderStorageBuffer(const ShaderStorageBufferDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ShaderStorageBuffer() override = default;

        void release() override;

        void bind() override;

        void bindShaderResource(u32 slot) override;

        void setData(const void* data, Size size) override;

        void copyFrom(const ResourceView<ShaderStorageBuffer>& other) override;

        [[nodiscard]] UP<UByte[]> getData() const override;

        [[nodiscard]] u64 getIdentifier() const override;

        [[nodiscard]] const std::string& getName() const override;

        [[nodiscard]] Size getSize() const override;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const override;

    private:

    };
}

#endif