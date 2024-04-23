#include "D3D11Sampler.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11Sampler::D3D11Sampler(const SamplerDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context):
    Sampler(desc),
    m_Sampler(nullptr),
    m_Context(context),
    m_Device(device){
        createResources();
    }

    D3D11Sampler::~D3D11Sampler() {
        if (m_Sampler) {
            m_Sampler->Release();
        }
    }

    void D3D11Sampler::bindShaderResource(uint32 slot) {
        m_Context->PSSetSamplers(slot, 1, &m_Sampler);
    }

    void D3D11Sampler::setFilter(SR_TEXTURE_FILTER minFilter, SR_TEXTURE_FILTER magFilter) {
        m_MinFilter = minFilter;
        m_MagFilter = magFilter;
        createResources();
    }

    void D3D11Sampler::setWrap(SR_TEXTURE_WRAP wrapU, SR_TEXTURE_WRAP wrapV, SR_TEXTURE_WRAP wrapW) {
        m_WrapU = wrapU;
        m_WrapV = wrapV;
        m_WrapW = wrapW;
        createResources();
    }

    void D3D11Sampler::setComparisonFunc(SR_COMPARISON_FUNC comparisonFunc) {
        m_ComparisonFunc = comparisonFunc;
        createResources();
    }

    void D3D11Sampler::setBorderColor(const float *borderColor) {
        m_BorderColor[0] = borderColor[0];
        m_BorderColor[1] = borderColor[1];
        m_BorderColor[2] = borderColor[2];
        m_BorderColor[3] = borderColor[3];
        createResources();
    }

    void D3D11Sampler::setLOD(float minLOD, float maxLOD, float LODBias) {
        m_MinLOD = minLOD;
        m_MaxLOD = maxLOD;
        m_LODBias = LODBias;
        createResources();
    }

    uint64 D3D11Sampler::getIdentifier() {
        return reinterpret_cast<uint64>(m_Sampler);
    }

    void D3D11Sampler::createResources() {
        if (m_Sampler) {
            m_Sampler->Release();
        }
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = getD3d11TextureFilter(m_MinFilter, m_MagFilter);
        samplerDesc.AddressU = getD3d11TextureAddressMode(m_WrapU);
        samplerDesc.AddressV = getD3d11TextureAddressMode(m_WrapV);
        samplerDesc.AddressW = getD3d11TextureAddressMode(m_WrapU); // D3D11 expects this field to be filled in, even if it is not used
        samplerDesc.MipLODBias = m_LODBias;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = getD3d11ComparisonFunc(m_ComparisonFunc);
        samplerDesc.BorderColor[0] = m_BorderColor[0];
        samplerDesc.BorderColor[1] = m_BorderColor[1];
        samplerDesc.BorderColor[2] = m_BorderColor[2];
        samplerDesc.BorderColor[3] = m_BorderColor[3];
        samplerDesc.MinLOD = m_MinLOD;
        samplerDesc.MaxLOD = m_MaxLOD;
        SR_CORE_D3D11_CALL(m_Device->CreateSamplerState(&samplerDesc, &m_Sampler));
    }

}

#endif


