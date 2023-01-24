#pragma once

#include "../../../../include/SyriusCore/Context/Viewport.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11Viewport : public Viewport{
    public:
        D3D11Viewport(const ViewportDesc& desc, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

        ~D3D11Viewport() override;

        void bind() override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_DeviceContext;
        D3D11_VIEWPORT m_Viewport;

    };

}

#endif