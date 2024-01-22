#include "D3D11Sampler.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11Sampler::D3D11Sampler(const SamplerDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context)
    : Sampler(desc),
      m_Context(context),
      m_Device(device){

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = getD3d11TextureFilter(desc.minFilter, desc.magFilter);
        samplerDesc.AddressU = getD3d11TextureAddressMode(desc.wrapU);
        samplerDesc.AddressV = getD3d11TextureAddressMode(desc.wrapV);
        samplerDesc.AddressW = getD3d11TextureAddressMode(desc.wrapU); // D3D11 expects this field to be filled in, even if it is not used
        samplerDesc.MipLODBias = desc.LODBias;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = getD3d11ComparisonFunc(desc.comparisonFunc);
        samplerDesc.BorderColor[0] = desc.borderColor[0];
        samplerDesc.BorderColor[1] = desc.borderColor[1];
        samplerDesc.BorderColor[2] = desc.borderColor[2];
        samplerDesc.BorderColor[3] = desc.borderColor[3];
        samplerDesc.MinLOD = desc.minLOD;
        samplerDesc.MaxLOD = desc.maxLOD;
        SR_CORE_D3D11_CALL(m_Device->CreateSamplerState(&samplerDesc, &m_Sampler));

    }

    D3D11Sampler::~D3D11Sampler() {
        if (m_Sampler) {
            m_Sampler->Release();
        }
    }

    void D3D11Sampler::bindShaderResource(uint32 slot) {
        m_Context->PSSetSamplers(slot, 1, &m_Sampler);
    }

    uint64 D3D11Sampler::getIdentifier() {
        return reinterpret_cast<uint64>(m_Sampler);
    }
}

#endif


