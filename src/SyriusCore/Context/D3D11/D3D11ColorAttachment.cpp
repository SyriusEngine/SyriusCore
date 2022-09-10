#include "D3D11ColorAttachment.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11ColorAttachment::D3D11ColorAttachment(const ColorAttachmentDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext)
    : ColorAttachment(desc),
    m_Device(device),
    m_Context(deviceContext),
    m_Texture(nullptr),
    m_RenderTargetView(nullptr),
    m_TextureView(nullptr),
    m_Sampler(nullptr){

        SR_TEXTURE_FORMAT sF = getTextureFormat(desc.m_Format);
        auto channelCount = getTextureChannelCount(sF);

        D3D11_TEXTURE2D_DESC textureDesc = { 0 };
        textureDesc.Width = desc.m_Width;
        textureDesc.Height = desc.m_Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = getFormat(channelCount);
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        SR_D3D11_CALL(m_Device->CreateTexture2D(&textureDesc, nullptr, &m_Texture));

        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {  };
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;

        SR_D3D11_CALL(m_Device->CreateRenderTargetView(m_Texture, &renderTargetViewDesc, &m_RenderTargetView));

        D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc = {  };
        textureViewDesc.Format = textureDesc.Format;
        textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        textureViewDesc.Texture2D.MipLevels = 1;
        textureViewDesc.Texture2D.MostDetailedMip = 0;

        SR_D3D11_CALL(m_Device->CreateShaderResourceView(m_Texture, &textureViewDesc, &m_TextureView));

        D3D11_SAMPLER_DESC samplerDesc = {  };
        samplerDesc.Filter = getD3d11TextureFilter(desc.m_MinFilter, desc.m_MagFilter);
        samplerDesc.AddressU = getD3d11TextureAddressMode(desc.m_WrapAddressU);
        samplerDesc.AddressV = getD3d11TextureAddressMode(desc.m_WrapAddressV);
        samplerDesc.AddressW = getD3d11TextureAddressMode(desc.m_WrapAddressU);

        SR_D3D11_CALL(m_Device->CreateSamplerState(&samplerDesc, &m_Sampler));

    }

    D3D11ColorAttachment::~D3D11ColorAttachment() {
        if (m_Sampler) {
            m_Sampler->Release();
        }
        if (m_TextureView) {
            m_TextureView->Release();
        }
        if (m_RenderTargetView) {
            m_RenderTargetView->Release();
        }
        if (m_Texture) {
            m_Texture->Release();
        }
    }

    void D3D11ColorAttachment::bind(uint32 slot) {
        m_Context->PSSetShaderResources(slot, 1, &m_TextureView);
        m_Context->PSSetSamplers(slot, 1, &m_Sampler);
    }

    Image *D3D11ColorAttachment::getData() {
        return nullptr;
    }

    void D3D11ColorAttachment::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        if (m_Texture) {
            m_Texture->Release();
        }

        SR_TEXTURE_FORMAT sF = getTextureFormat(m_Format);
        auto channelCount = getTextureChannelCount(sF);

        D3D11_TEXTURE2D_DESC textureDesc = { 0 };
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = getFormat(channelCount);
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        SR_D3D11_CALL(m_Device->CreateTexture2D(&textureDesc, nullptr, &m_Texture));

    }

    uint64 D3D11ColorAttachment::getIdentifier() const {
        return reinterpret_cast<uint64>(m_Texture);;
    }

    DXGI_FORMAT D3D11ColorAttachment::getFormat(uint32 channelCount) {
        switch (channelCount) {
            case 1:
                return DXGI_FORMAT_R8_UNORM;
            case 2:
                return DXGI_FORMAT_R8G8_UNORM;
            case 4:
                return DXGI_FORMAT_R8G8B8A8_UNORM;
            default: {
                SR_CORE_WARNING("Unknown channel count: " + std::to_string(channelCount));
                return DXGI_FORMAT_R8G8B8A8_UNORM;
            }
        }
    }

    ID3D11RenderTargetView *D3D11ColorAttachment::getRenderTargetView() const {
        return m_RenderTargetView;
    }

}

#endif
