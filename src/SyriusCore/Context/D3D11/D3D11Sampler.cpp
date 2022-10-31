#include "D3D11Sampler.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11Sampler::D3D11Sampler(const SamplerDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context)
    : Sampler(desc),
      m_Context(context),
      m_Device(device){

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = getD3d11TextureFilter(desc.m_MinFilter, desc.m_MagFilter);
        samplerDesc.AddressU = getD3d11TextureAddressMode(desc.m_WrapU);
        samplerDesc.AddressV = getD3d11TextureAddressMode(desc.m_WrapV);
        samplerDesc.AddressW = getD3d11TextureAddressMode(desc.m_WrapU); // D3D11 expects this field to be filled in, even if it is not used
        samplerDesc.MipLODBias = desc.m_LODBias;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = getD3d11ComparisonFunc(desc.m_ComparisonFunc);
        samplerDesc.BorderColor[0] = desc.m_BorderColor[0];
        samplerDesc.BorderColor[1] = desc.m_BorderColor[1];
        samplerDesc.BorderColor[2] = desc.m_BorderColor[2];
        samplerDesc.BorderColor[3] = desc.m_BorderColor[3];
        samplerDesc.MinLOD = desc.m_MinLOD;
        samplerDesc.MaxLOD = desc.m_MaxLOD;
        SR_D3D11_CALL(m_Device->CreateSamplerState(&samplerDesc, &m_Sampler));

    }

    D3D11Sampler::~D3D11Sampler() {
        if (m_Sampler) {
            m_Sampler->Release();
        }
    }

    void D3D11Sampler::bind(uint32 slot) {
        m_Context->PSSetSamplers(slot, 1, &m_Sampler);
    }

    uint64 D3D11Sampler::getIdentifier() {
        return reinterpret_cast<uint64>(m_Sampler);
    }
}

#endif


