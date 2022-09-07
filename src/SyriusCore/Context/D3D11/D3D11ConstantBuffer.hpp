#pragma once

#include "../../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11ConstantBufferBase: public ConstantBuffer{
    public:
        D3D11ConstantBufferBase(const ConstantBufferDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ConstantBufferBase() override;

        void setData(const void* data) override;

        void addShader(Shader* shader) override;

        [[nodiscard]] uint64 getIdentifier() const override;

    protected:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;
        ID3D11Buffer* m_Buffer;
    };

    class D3D11ConstantBufferVertex: public D3D11ConstantBufferBase{
    public:
        D3D11ConstantBufferVertex(const ConstantBufferDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ConstantBufferVertex() override = default;

        void bind() override;
    };

    class D3D11ConstantBufferPixel: public D3D11ConstantBufferBase{
    public:
        D3D11ConstantBufferPixel(const ConstantBufferDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ConstantBufferPixel() override = default;

        void bind() override;
    };

    class D3D11ConstantBufferGeometry: public D3D11ConstantBufferBase{
    public:
        D3D11ConstantBufferGeometry(const ConstantBufferDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ConstantBufferGeometry() override = default;

        void bind() override;
    };

}

#endif
