#include "D3D11Texture2D.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11Texture2D::D3D11Texture2D(const Texture2DDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context)
    : Texture2D(desc),
    m_Device(device),
    m_Context(context),
    m_Texture(nullptr),
    m_TextureView(nullptr){
        SR_CORE_PRECONDITION(desc.data != nullptr, "Texture data must not be null");

        auto channelCount = getTextureDataChannelCount(desc.format);
        if (channelCount == 3){
            SR_CORE_EXCEPTION("D3D11 does not support 3 channel textures, please supply data in format with 1, 2 or 4 channels");
        }

        D3D11_TEXTURE2D_DESC textureDesc = { 0 };
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = getFormat(channelCount);
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subresourceData = { 0 };
        subresourceData.SysMemPitch = m_Width * (sizeof(ubyte) * channelCount);
        subresourceData.pSysMem = desc.data;

        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&textureDesc, &subresourceData, &m_Texture));

        D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc = {  };
        textureViewDesc.Format = textureDesc.Format;
        textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        textureViewDesc.Texture2D.MipLevels = 1;
        textureViewDesc.Texture2D.MostDetailedMip = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateShaderResourceView(m_Texture, &textureViewDesc, &m_TextureView));
    }

    D3D11Texture2D::D3D11Texture2D(const Texture2DImageDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context)
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

    void D3D11Texture2D::setData(const void *data, uint32 x, uint32 y, uint32 width, uint32 height) {
        SR_CORE_PRECONDITION(data != nullptr, "Data must not be null");
        SR_CORE_PRECONDITION(x < m_Width, "x is out of range");
        SR_CORE_PRECONDITION(y < m_Height, "y is out of range");
        SR_CORE_PRECONDITION(x + width <= m_Width, "x + width is out of range");
        SR_CORE_PRECONDITION(y + height <= m_Height, "y + height is out of range");

        // TODO: Test this code because dont know if it works
        auto channelCount = getTextureDataChannelCount(m_Format);
        D3D11_BOX box = { 0 };
        box.left = x;
        box.right = x + width;
        box.top = y;
        box.bottom = y + height;
        box.front = 0;
        box.back = 1;

        m_Context->UpdateSubresource(m_Texture, 0, &box, data, width * (sizeof(ubyte) * channelCount), 0);
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
