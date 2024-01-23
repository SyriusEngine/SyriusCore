#pragma once

#include "../../../../include/SyriusCore/Context/ShaderStorageBuffer.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11ShaderStorageBuffer : public ShaderStorageBuffer{
    public:
        D3D11ShaderStorageBuffer(const ShaderStorageBufferDesc& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ShaderStorageBuffer() override;

        void bind() override;

        void bindShaderResource(uint32 slot) override;

        void setData(const void* data, uint32 size) override;

        [[nodiscard]] Resource<byte[]> getData() const override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;
        ID3D11Buffer* m_Buffer;

    };

}

#endif