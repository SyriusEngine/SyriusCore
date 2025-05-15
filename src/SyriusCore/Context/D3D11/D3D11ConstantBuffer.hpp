#pragma once

#include "../../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "Internal/D3D11Buffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11ConstantBufferBase: public ConstantBuffer{
    public:
        D3D11ConstantBufferBase(const ConstantBufferDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ConstantBufferBase() override = default;

        void release() override;

        void bind() override;

        void setData(const void* data, u64 size) override;

        void copyFrom(const ResourceView<ConstantBuffer>& other) override;

        [[nodiscard]] UP<UByte[]> getData() const override;

        [[nodiscard]] u64 getIdentifier() const override;

        [[nodiscard]] const std::string& getName() const override;

        [[nodiscard]] Size getSize() const override;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const override;

    protected:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;
        D3D11Buffer m_Buffer;
    };

    class D3D11ConstantBufferVertex: public D3D11ConstantBufferBase{
    public:
        D3D11ConstantBufferVertex(const ConstantBufferDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ConstantBufferVertex() override = default;

        void bindShaderResource(u32 slot) override;
    };

    class D3D11ConstantBufferPixel: public D3D11ConstantBufferBase{
    public:
        D3D11ConstantBufferPixel(const ConstantBufferDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ConstantBufferPixel() override = default;

        void bindShaderResource(u32 slot) override;
    };

    class D3D11ConstantBufferGeometry: public D3D11ConstantBufferBase{
    public:
        D3D11ConstantBufferGeometry(const ConstantBufferDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ConstantBufferGeometry() override = default;

        void bindShaderResource(u32 slot) override;
    };

}

#endif
