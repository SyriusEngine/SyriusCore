#pragma once

#include "../../../../include/SyriusCore/Context/ShaderStorageBuffer.hpp"
#include "Internal/D3D11Buffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius {

    class D3D11ShaderStorageBufferBase: public ShaderStorageBuffer{
    public:
        D3D11ShaderStorageBufferBase(const ShaderStorageBufferDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ShaderStorageBufferBase() override;

        void release() override;

        void bind() override;

        void setData(const void* data, Size size) override;

        void copyFrom(const ResourceView<ShaderStorageBuffer>& other) override;

        [[nodiscard]] UP<UByte[]> getData() const override;

        [[nodiscard]] u64 getIdentifier() const override;

        [[nodiscard]] const std::string& getName() const override;

        [[nodiscard]] Size getSize() const override;

        [[nodiscard]] Size getCount() const override;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const override;

    protected:

        void create_srv();

        void release_srv() const;

    protected:
        ID3D11Device* m_Device = nullptr;
        ID3D11DeviceContext* m_Context = nullptr;
        ID3D11ShaderResourceView* m_ShaderResourceView = nullptr; //SRV
        D3D11Buffer m_Buffer;
    };

    class D3D11ShaderStorageBufferVertex: public D3D11ShaderStorageBufferBase {
    public:
        D3D11ShaderStorageBufferVertex(const ShaderStorageBufferDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ShaderStorageBufferVertex() override = default;

        void bindShaderResource(u32 slot) override;
    };

    class D3D11ShaderStorageBufferFragment: public D3D11ShaderStorageBufferBase {
    public:
        D3D11ShaderStorageBufferFragment(const ShaderStorageBufferDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ShaderStorageBufferFragment() override = default;

        void bindShaderResource(u32 slot) override;
    };

    class D3D11ShaderStorageBufferGeometry: public D3D11ShaderStorageBufferBase {
    public:
        D3D11ShaderStorageBufferGeometry(const ShaderStorageBufferDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ShaderStorageBufferGeometry() override = default;

        void bindShaderResource(u32 slot) override;
    };
}

#endif