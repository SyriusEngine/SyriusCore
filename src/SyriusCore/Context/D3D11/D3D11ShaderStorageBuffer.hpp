#pragma once

#include "../../../../include/SyriusCore/Context/ShaderStorageBuffer.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11ShaderStorageBufferBase : public ShaderStorageBuffer{
    public:
        D3D11ShaderStorageBufferBase(const ShaderStorageBufferDesc& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ShaderStorageBufferBase() override;

        void bind() override;

        void setData(const void* data, uint32 size) override;

        [[nodiscard]] Resource<ubyte[]> getData() const override;

        [[nodiscard]] uint64 getIdentifier() const override;

    protected:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;
        ID3D11Buffer* m_Buffer;

        ID3D11ShaderResourceView* m_View;

    };

    class D3D11ShaderStorageBufferVertex : public D3D11ShaderStorageBufferBase{
    public:
        D3D11ShaderStorageBufferVertex(const ShaderStorageBufferDesc& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ShaderStorageBufferVertex() override;

        void bindShaderResource(uint32 slot) override;

    };

    class D3D11ShaderStorageBufferPixel : public D3D11ShaderStorageBufferBase{
    public:
        D3D11ShaderStorageBufferPixel(const ShaderStorageBufferDesc& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ShaderStorageBufferPixel() override;

        void bindShaderResource(uint32 slot) override;

    };

}

#endif