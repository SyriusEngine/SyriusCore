#include "D3D11ColorAttachment.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius {

    D3D11ColorAttachment::D3D11ColorAttachment(const ColorAttachmentDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *deviceContext) :
            ColorAttachment(desc, deviceLimits),
            m_Device(device),
            m_Context(deviceContext),
            m_Texture(nullptr),
            m_RenderTargetView(nullptr),
            m_TextureView(nullptr) {
        SR_LOG_ERROR_IF_FALSE(m_DeviceLimits->texture2DFormatSupported(desc.format), "D3D11Texture2D", "Supplied texture format {} is not supported by the device", desc.format);

        createResources();
    }

    D3D11ColorAttachment::~D3D11ColorAttachment() {
        destroyResources();
    }

    void D3D11ColorAttachment::bind() {
        // D3D11 doesn't have a bind function
    }

    void D3D11ColorAttachment::bindShaderResource(u32 slot) {
        SR_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[D3D11ColorAttachment]: Supplied slot {} is greater than the device number of texture slots {}", slot, m_DeviceLimits->getMaxTextureSlots());

        m_Context->PSSetShaderResources(slot, 1, &m_TextureView);
    }

    void D3D11ColorAttachment::clear() {
        m_Context->ClearRenderTargetView(m_RenderTargetView, m_ClearColor);
    }

    void D3D11ColorAttachment::onResize(u32 width, u32 height) {
        destroyResources();
        m_Width = width;
        m_Height = height;
        createResources();
    }

    UP<Image> D3D11ColorAttachment::getData() {
        // we use a staging texture to copy the data back to the CPU as a texture cannot be mapped
        auto channelCount = getTextureChannelCount(m_Format);
        auto bpp = getBytesPerPixel(m_Format);

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

        auto* data = reinterpret_cast<u8*>(mappedResource.pData);
        ImageDesc imgDesc;
        imgDesc.width = desc.Width;
        imgDesc.height = desc.Height;
        imgDesc.format = m_Format;
        imgDesc.data = nullptr;
        auto img = createImage(imgDesc);

        auto imgData = reinterpret_cast<u8*>(img->getData());
        // remove added padding data (if any), this is added by D3D11 to properly align the data
        size_t rowPitch = mappedResource.RowPitch;
        size_t destRowPitch = desc.Width * bpp;

        for (u32 y = 0; y < desc.Height; ++y) {
            // Copy each row considering the padding in the source data
            memcpy(imgData + y * destRowPitch, data + y * rowPitch, desc.Width * bpp);
        }

        m_Context->Unmap(stagingTexture, 0);
        stagingTexture->Release();
        return std::move(img);
    }

    u64 D3D11ColorAttachment::getIdentifier() const {
        return reinterpret_cast<u64>(m_Texture);
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
                                                             const UP<DeviceLimits>& deviceLimits,
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

    void D3D11DefaultColorAttachment::bindShaderResource(u32 slot) {

    }

    void D3D11DefaultColorAttachment::clear() {
        m_Context->ClearRenderTargetView(m_RenderTargetView, m_ClearColor);
    }

    void D3D11DefaultColorAttachment::onResize(u32 width, u32 height) {
        m_Context->OMSetRenderTargets(0, nullptr, nullptr);
        if (m_RenderTargetView) {
            m_RenderTargetView->Release();
        }
        m_RenderTargetView = nullptr;

        SR_CORE_D3D11_CALL(m_SwapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

        createResources();
    }

    UP<Image> D3D11DefaultColorAttachment::getData() {
        return {};
    }

    u64 D3D11DefaultColorAttachment::getIdentifier() const {
        return 0;
    }

    ID3D11RenderTargetView *D3D11DefaultColorAttachment::getRenderTargetView() const {
        return m_RenderTargetView;
    }

    void D3D11DefaultColorAttachment::createResources() {
        ID3D11Texture2D *backBuffer = nullptr;
        SR_CORE_D3D11_CALL(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer)));

        if (!backBuffer) {
            SR_LOG_THROW("D3D11ColorAttachment", "Failed to get back buffer from swap chain");
        }

        SR_CORE_D3D11_CALL(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView));

        // as the buffer is maintained by the swap chain, get the creation desc from the swap chain
        D3D11_TEXTURE2D_DESC t2dDesc;
        backBuffer->GetDesc(&t2dDesc);
        m_Width = t2dDesc.Width;
        m_Height = t2dDesc.Height;

        backBuffer->Release();
    }
}

#endif
