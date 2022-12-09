#include "D3D11Shader.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11Shader::D3D11Shader(const ShaderDesc& desc, ID3D11Device *device, ID3D11DeviceContext *context)
    : Shader(desc),
    m_Device(device),
    m_Context(context),
    m_PixelShader(nullptr),
    m_VertexShader(nullptr){

        auto vsBlob = reinterpret_cast<ID3DBlob*>(desc.m_VertexShader->getIdentifier());
        auto psBlob = reinterpret_cast<ID3DBlob*>(desc.m_FragmentShader->getIdentifier());

        SR_D3D11_CALL(m_Device->CreateVertexShader(
                vsBlob->GetBufferPointer(),
                vsBlob->GetBufferSize(),
                nullptr,
                &m_VertexShader
                ));
        SR_D3D11_CALL(m_Device->CreatePixelShader(
                psBlob->GetBufferPointer(),
                psBlob->GetBufferSize(),
                nullptr,
                &m_PixelShader
                ));

    }

    D3D11Shader::~D3D11Shader() {
        if (m_VertexShader){
            m_VertexShader->Release();
        }
        if (m_PixelShader){
            m_PixelShader->Release();
        }
    }

    void D3D11Shader::bind() {
        m_Context->VSSetShader(m_VertexShader, nullptr, 0);
        m_Context->PSSetShader(m_PixelShader, nullptr, 0);
    }

    uint64 D3D11Shader::getIdentifier() const {
        SR_CORE_WARNING("D3D11Shader has no unique identifier");
        return 0;
    }
}

#endif