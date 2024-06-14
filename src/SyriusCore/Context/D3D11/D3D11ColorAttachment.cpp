#include "D3D11ColorAttachment.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius {

    D3D11ColorAttachment::D3D11ColorAttachment(const ColorAttachmentDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *deviceContext) :
            ColorAttachment(desc, deviceLimits),
            m_Device(device),
            m_Context(deviceContext),
            m_Texture(nullptr),
            m_RenderTargetView(nullptr),
            m_TextureView(nullptr) {
        if (!m_DeviceLimits->texture2DFormatSupported(desc.format)){
            SR_CORE_THROW("[D3D11Texture2D]: Supplied texture format (%i) is not supported by the device", desc.format);
        }
        createResources();
    }

    D3D11ColorAttachment::~D3D11ColorAttachment() {
        destroyResources();
    }

    void D3D11ColorAttachment::bind() {
        // D3D11 doesn't have a bind function
    }

    void D3D11ColorAttachment::bindShaderResource(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[D3D11ColorAttachment]: Supplied slot (%i) is greater than the device number of texture slots (%i)", slot, m_DeviceLimits->getMaxTextureSlots());

        m_Context->PSSetShaderResources(slot, 1, &m_TextureView);
    }

    void D3D11ColorAttachment::clear() {
        m_Context->ClearRenderTargetView(m_RenderTargetView, m_ClearColor);
    }

    void D3D11ColorAttachment::onResize(uint32 width, uint32 height) {
        destroyResources();
        m_Width = width;
        m_Height = height;
        createResources();
    }

    Resource<Image> D3D11ColorAttachment::getData() {
        // we use a staging texture to copy the data back to the CPU as a texture cannot be mapped
        auto channelCount = getTextureChannelCount(m_Format);

        D3D11_TEXTURE2D_DESC desc;
        m_Texture->GetDesc(&desc);
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.BindFlags = 0;
        desc.MiscFlags = 0;
        ID3D11Texture2D* stagingTexture = nullptr;
        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&desc, nullptr, &stagingTexture));

        m_Context->CopyResource(stagingTexture, m_Texture);

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        SR_CORE_D3D11_CALL(m_Context->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource));

        auto* data = static_cast<float*>(mappedResource.pData);
        ImageUI8Desc imgDesc;
        imgDesc.width = desc.Width;
        imgDesc.height = desc.Height;
        switch (channelCount) {
            case 1: imgDesc.format = SR_TEXTURE_R_UI8; break;
            case 2: imgDesc.format = SR_TEXTURE_RG_UI8; break;
            case 3: imgDesc.format = SR_TEXTURE_RGB_UI8; break;
            case 4: imgDesc.format = SR_TEXTURE_RGBA_UI8; break;
            default: SR_CORE_THROW("[D3D11ColorAttachment]: Invalid channel count %i", channelCount);
        }
        imgDesc.data = nullptr;
        auto img = createImage(imgDesc);

        auto imgData = reinterpret_cast<uint8*>(img->getData());
        size_t rowPitch = mappedResource.RowPitch / sizeof(float);
        // remove added padding data (if any), this is added by D3D11 to properly align the data
        for (uint32 y = 0; y < desc.Height; ++y) {
            for (uint32 x = 0; x < desc.Width; ++x) {
                for (uint32 c = 0; c < channelCount; ++c) {
                    size_t srcIndex = (y * rowPitch) + (x * channelCount) + c;
                    size_t destIndex = (y * desc.Width * channelCount) + (x * channelCount) + c;
                    imgData[destIndex] = static_cast<uint8>(data[srcIndex] * 255.0f);
                }
            }
        }

        m_Context->Unmap(stagingTexture, 0);
        stagingTexture->Release();
        return std::move(img);
    }

    uint64 D3D11ColorAttachment::getIdentifier() const {
        return reinterpret_cast<uint64>(m_Texture);
    }

    ID3D11RenderTargetView *D3D11ColorAttachment::getRenderTargetView() const {
        return m_RenderTargetView;
    }

    void  D3D11ColorAttachment::createResources(){
        D3D11_TEXTURE2D_DESC textureDesc = {0};
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = getD3d11TextureFormat(m_Format);
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&textureDesc, nullptr, &m_Texture));

        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateRenderTargetView(m_Texture, &renderTargetViewDesc, &m_RenderTargetView));

        D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc = {};
        textureViewDesc.Format = textureDesc.Format;
        textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        textureViewDesc.Texture2D.MipLevels = 1;
        textureViewDesc.Texture2D.MostDetailedMip = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateShaderResourceView(m_Texture, &textureViewDesc, &m_TextureView));
    }

    void  D3D11ColorAttachment::destroyResources(){
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

    D3D11DefaultColorAttachment::D3D11DefaultColorAttachment(const ColorAttachmentDesc &desc,
                                                             const Resource<DeviceLimits>& deviceLimits,
                                                             ID3D11Device *device,
                                                             ID3D11DeviceContext *deviceContext,
                                                             IDXGISwapChain *swapChain) :
            ColorAttachment(desc, deviceLimits),
            m_Device(device),
            m_Context(deviceContext),
            m_SwapChain(swapChain),
            m_RenderTargetView(nullptr) {
        createResources();

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

    void D3D11DefaultColorAttachment::onResize(uint32 width, uint32 height) {
        m_Context->OMSetRenderTargets(0, nullptr, nullptr);
        if (m_RenderTargetView) {
            m_RenderTargetView->Release();
        }
        m_RenderTargetView = nullptr;

        SR_CORE_D3D11_CALL(m_SwapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

        createResources();
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

    void D3D11DefaultColorAttachment::createResources() {
        ID3D11Texture2D *backBuffer = nullptr;
        SR_CORE_D3D11_CALL(
                m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer)));
        if (backBuffer) {
            SR_CORE_D3D11_CALL(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView));

            // as the buffer is maintained by the swap chain, get the creation desc from the swap chain
            D3D11_TEXTURE2D_DESC t2dDesc;
            backBuffer->GetDesc(&t2dDesc);
            m_Width = t2dDesc.Width;
            m_Height = t2dDesc.Height;

            backBuffer->Release();

        } else {
            SR_CORE_THROW("[D3D11ColorAttachment]: Failed to get back buffer from swap chain %p", m_SwapChain);
        }
    }
}

#endif
