#pragma once

#include "../../../../include/SyriusCore/Context/IndexBuffer.hpp"
#include "Internal/D3D11Buffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11IndexBuffer: public IndexBuffer{
    public:
        D3D11IndexBuffer(const IndexBufferDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11IndexBuffer() override = default;

        void release() override;

        void bind() override;

        void setData(const void* data, u64 count) override;

        void copyFrom(const ResourceView<IndexBuffer>& other) override;

        [[nodiscard]] UP<UByte[]> getData() const override;

        [[nodiscard]] u64 getIdentifier() const override;

        [[nodiscard]] Size getSize() const override;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const override;

    private:
        ID3D11Device* m_Device = nullptr;
        ID3D11DeviceContext* m_Context = nullptr;
        D3D11Buffer m_Buffer;

        DXGI_FORMAT m_DxgiDataType;

    };


}

#endif

