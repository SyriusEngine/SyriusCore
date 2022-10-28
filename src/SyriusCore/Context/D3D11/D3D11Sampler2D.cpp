#include "D3D11Sampler2D.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11Sampler2D::D3D11Sampler2D(const Sampler2DDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context)
    : Sampler2D(desc),
    m_Context(context),
    m_Device(device){

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = getD3d11TextureFilter(desc.m_MinFilter, desc.m_MagFilter);
        samplerDesc.AddressU = getD3d11TextureAddressMode(desc.m_WrapU);
        samplerDesc.AddressV = getD3d11TextureAddressMode(desc.m_WrapV);
        SR_D3D11_CALL(m_Device->CreateSamplerState(&samplerDesc, &m_Sampler));

    }

    D3D11Sampler2D::~D3D11Sampler2D() {
        if (m_Sampler) {
            m_Sampler->Release();
        }
    }

    void D3D11Sampler2D::bind(uint32 slot) {
        m_Context->PSSetSamplers(slot, 1, &m_Sampler);
    }

    uint64 D3D11Sampler2D::getIdentifier() {
        return reinterpret_cast<uint64>(m_Sampler);
    }
}

#endif


