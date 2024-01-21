#pragma once

#include "../../../../include/SyriusCore/Context/VertexBuffer.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11VertexBuffer: public VertexBuffer{
    public:
        D3D11VertexBuffer(const VertexBufferDesc& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11VertexBuffer() override;

        void bind() override;

        void setData(const void* data) override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11Buffer* m_Buffer;


    };

}

#endif
