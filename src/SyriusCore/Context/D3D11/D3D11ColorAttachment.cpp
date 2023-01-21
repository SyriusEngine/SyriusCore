#include "D3D11ColorAttachment.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius {

    D3D11ColorAttachment::D3D11ColorAttachment(const ColorAttachmentDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext) :
    ColorAttachment(desc),
    m_Device(device),
    m_Context(deviceContext),
    m_ColorBuffer(nullptr),
    m_RenderTargetView(nullptr),
    m_BufferView(nullptr) {
        SR_CORE_PRECONDITION(desc.format != SR_TEXTURE_DATA_FORMAT_DEPTH_16 and
                             desc.format != SR_TEXTURE_DATA_FORMAT_DEPTH_24 and
                             desc.format != SR_TEXTURE_DATA_FORMAT_DEPTH_32 and
                             desc.format != SR_TEXTURE_DATA_FORMAT_DEPTH_24_STENCIL_8 and
                             desc.format != SR_TEXTURE_DATA_FORMAT_DEPTH_32_STENCIL_8,
                             "Depth/stencil format is not supported for color attachment");

        SR_TEXTURE_FORMAT sF = getTextureFormat(desc.format);
        auto channelCount = getTextureChannelCount(sF);

        D3D11_TEXTURE2D_DESC textureDesc = {0};
        textureDesc.Width = desc.width;
        textureDesc.Height = desc.height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = getFormat(channelCount);
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&textureDesc, nullptr, &m_ColorBuffer));

        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateRenderTargetView(m_ColorBuffer, &renderTargetViewDesc, &m_RenderTargetView));

        D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc = {};
        textureViewDesc.Format = textureDesc.Format;
        textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        textureViewDesc.Texture2D.MipLevels = 1;
        textureViewDesc.Texture2D.MostDetailedMip = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateShaderResourceView(m_ColorBuffer, &textureViewDesc, &m_BufferView));

    }

    D3D11ColorAttachment::~D3D11ColorAttachment() {
        if (m_BufferView) {
            m_BufferView->Release();
        }
        if (m_RenderTargetView) {
            m_RenderTargetView->Release();
        }
        if (m_ColorBuffer) {
            m_ColorBuffer->Release();
        }
    }

    void D3D11ColorAttachment::bind() {

    }

    void D3D11ColorAttachment::bindShaderResource(uint32 slot) {
        m_Context->PSSetShaderResources(slot, 1, &m_BufferView);
    }

    void D3D11ColorAttachment::clear() {
        m_Context->ClearRenderTargetView(m_RenderTargetView, m_ClearColor);
    }

    void D3D11ColorAttachment::setSize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        if (m_ColorBuffer) {
            m_ColorBuffer->Release();
        }

        SR_TEXTURE_FORMAT sF = getTextureFormat(m_Format);
        auto channelCount = getTextureChannelCount(sF);

        D3D11_TEXTURE2D_DESC textureDesc = {0};
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

        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&textureDesc, nullptr, &m_ColorBuffer));
    }

    Resource<Image> D3D11ColorAttachment::getData() {
        return Resource<Image>();
    }

    uint64 D3D11ColorAttachment::getIdentifier() const {
        return reinterpret_cast<uint64>(m_ColorBuffer);
    }

    ID3D11RenderTargetView *D3D11ColorAttachment::getRenderTargetView() const {
        return m_RenderTargetView;
    }

    DXGI_FORMAT D3D11ColorAttachment::getFormat(uint32 channelCount) {
        switch (channelCount) {
            case 1:
                return DXGI_FORMAT_R32_FLOAT;
            case 2:
                return DXGI_FORMAT_R32G32_FLOAT;
            case 3:
                return DXGI_FORMAT_R32G32B32_FLOAT;
            case 4:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            default: {
                SR_CORE_WARNING("Unknown channel count: " + std::to_string(channelCount));
                return DXGI_FORMAT_R8G8B8A8_UNORM;
            }
        }
    }

    D3D11DefaultColorAttachment::D3D11DefaultColorAttachment(const ColorAttachmentDesc &desc, ID3D11Device *device,
                                                             ID3D11DeviceContext *deviceContext,
                                                             IDXGISwapChain *swapChain) :
            ColorAttachment(desc),
            m_Device(device),
            m_Context(deviceContext),
            m_SwapChain(swapChain),
            m_RenderTargetView(nullptr) {
        ID3D11Texture2D *backBuffer = nullptr;
        SR_CORE_D3D11_CALL(
                m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer)));
        if (backBuffer) {
            SR_CORE_D3D11_CALL(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView));
            backBuffer->Release();

        } else {
            SR_CORE_EXCEPTION("Failed to get back buffer from swap chain");
        }

    }

    D3D11DefaultColorAttachment::~D3D11DefaultColorAttachment() {
        if (m_RenderTargetView){
            m_RenderTargetView->Release();
        }
    }

    void D3D11DefaultColorAttachment::bind() {

    }

    void D3D11DefaultColorAttachment::bindShaderResource(uint32 slot) {

    }

    void D3D11DefaultColorAttachment::clear() {
        m_Context->ClearRenderTargetView(m_RenderTargetView, m_ClearColor);
    }

    void D3D11DefaultColorAttachment::setSize(uint32 width, uint32 height) {
        m_Context->OMSetRenderTargets(0, nullptr, nullptr);
        if (m_RenderTargetView) {
            m_RenderTargetView->Release();
        }
        m_RenderTargetView = nullptr;

        SR_CORE_D3D11_CALL(m_SwapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

        ID3D11Texture2D *backBuffer = nullptr;
        SR_CORE_D3D11_CALL(
                m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer)));
        if (backBuffer) {
            SR_CORE_D3D11_CALL(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView));
            backBuffer->Release();

        } else {
            SR_CORE_EXCEPTION("Failed to get back buffer from swap chain");
        }
    }

    Resource<Image> D3D11DefaultColorAttachment::getData() {
        return Resource<Image>();
    }

    uint64 D3D11DefaultColorAttachment::getIdentifier() const {
        return 0;
    }

    ID3D11RenderTargetView *D3D11DefaultColorAttachment::getRenderTargetView() const {
        return m_RenderTargetView;
    }
}

#endif
