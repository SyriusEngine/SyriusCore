#pragma once

#include "../../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11ConstantBuffer: public ConstantBuffer{
    public:
        D3D11ConstantBuffer(const ConstantBufferDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ConstantBuffer() override;


    };

}

#endif
