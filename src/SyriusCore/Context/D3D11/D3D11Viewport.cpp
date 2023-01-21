#include "D3D11Viewport.hpp"

namespace Syrius{

    D3D11Viewport::D3D11Viewport(const ViewportDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext) :
    Viewport(desc),
    m_Device(device),
    m_DeviceContext(deviceContext) {
        m_Viewport.Width = static_cast<float>(m_Width);
        m_Viewport.Height = static_cast<float>(m_Height);
        m_Viewport.MinDepth = desc.minDepth;
        m_Viewport.MaxDepth = desc.maxDepth;
        m_Viewport.TopLeftX = static_cast<float>(m_XPos);
        m_Viewport.TopLeftY = static_cast<float>(m_YPos);
    }

    D3D11Viewport::~D3D11Viewport() {

    }

    void D3D11Viewport::bind() {
        m_DeviceContext->RSSetViewports(1, &m_Viewport);
    }
}
