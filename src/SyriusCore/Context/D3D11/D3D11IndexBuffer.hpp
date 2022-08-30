#pragma once

#include "../../../../include/SyriusCore/Context/IndexBuffer.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11IndexBuffer: public IndexBuffer{
    public:
        D3D11IndexBuffer(const IndexBufferDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11IndexBuffer() override;

        void bind() override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11Buffer* m_Buffer;

        DXGI_FORMAT m_DxgiDataType;

    };


}

#endif

