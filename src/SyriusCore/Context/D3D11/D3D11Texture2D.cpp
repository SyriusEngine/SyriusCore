#include "D3D11Texture2D.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11Texture2D::D3D11Texture2D(const Texture2DDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context)
    : Texture2D(desc),
    m_Device(device),
    m_Context(context),
    m_Texture(nullptr),
    m_TextureView(nullptr){
        if (desc.m_Image->getChannelCount() == 3){
            desc.m_Image->extendAlpha();
        }

        D3D11_TEXTURE2D_DESC textureDesc = { 0 };
        textureDesc.Width = desc.m_Image->getWidth();
        textureDesc.Height = desc.m_Image->getHeight();
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = getFormat(desc.m_Image->getChannelCount());
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subresourceData = { 0 };
        subresourceData.SysMemPitch = desc.m_Image->getWidth() * (sizeof(ubyte) * desc.m_Image->getChannelCount());
        subresourceData.pSysMem = &desc.m_Image->getData()[0];

        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&textureDesc, &subresourceData, &m_Texture));

        D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc = {  };
        textureViewDesc.Format = textureDesc.Format;
        textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        textureViewDesc.Texture2D.MipLevels = 1;
        textureViewDesc.Texture2D.MostDetailedMip = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateShaderResourceView(m_Texture, &textureViewDesc, &m_TextureView));
    }

    D3D11Texture2D::~D3D11Texture2D() {
        if (m_TextureView) {
            m_TextureView->Release();
        }
        if (m_Texture) {
            m_Texture->Release();
        }

    }

    void D3D11Texture2D::bind(uint32_t slot) {
        m_Context->PSSetShaderResources(slot, 1, &m_TextureView);
    }

    void D3D11Texture2D::unbind() {

    }

    uint64 D3D11Texture2D::getIdentifier() const {
        return reinterpret_cast<uint64>(m_Texture);
    }

    DXGI_FORMAT D3D11Texture2D::getFormat(uint32 channelcount) const {
        switch (channelcount) {
            case 1:
                return DXGI_FORMAT_R8_UNORM;
            case 2:
                return DXGI_FORMAT_R8G8_UNORM;
            case 4:
                return DXGI_FORMAT_R8G8B8A8_UNORM;
            default: {
                SR_CORE_WARNING("Unknown channel count: " + std::to_string(channelcount));
                return DXGI_FORMAT_R8G8B8A8_UNORM;
            }
        }
    }
}

#endif
