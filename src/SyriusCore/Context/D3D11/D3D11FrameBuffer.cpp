#include "D3D11FrameBuffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11FrameBuffer::D3D11FrameBuffer(const FrameBufferDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext)
    : FrameBuffer(desc),
    m_Device(device),
    m_DeviceContext(deviceContext),
    m_Nullable(desc.m_ColorAttachmentFormats.size(), nullptr),
    m_DepthStencilView(nullptr){
        m_Viewport.Width = static_cast<float>(m_Width);
        m_Viewport.Height = static_cast<float>(m_Height);
        m_Viewport.MinDepth = desc.m_MinDepth;
        m_Viewport.MaxDepth = desc.m_MaxDepth;
        m_Viewport.TopLeftX = static_cast<float>(m_XPos);
        m_Viewport.TopLeftY = static_cast<float>(m_YPos);

        for (auto& format: desc.m_ColorAttachmentFormats){
            // really annoying but yeah
            ColorAttachmentDesc caDesc;
            caDesc.m_Width = m_Width;
            caDesc.m_Height = m_Height;
            caDesc.m_Format = format;
            auto attachment = new D3D11ColorAttachment(caDesc, m_Device, m_DeviceContext);
            m_ColorAttachments.push_back(attachment);
            m_RenderTargetViews.push_back(attachment->getRenderTargetView());
            m_Nullable.emplace_back(nullptr);
        }

        if (desc.m_EnableDepthTest or desc.m_EnableStencilTest){
            DepthStencilAttachmentDesc dsaDesc;
            dsaDesc.m_Width = m_Width;
            dsaDesc.m_Height = m_Height;
            dsaDesc.m_EnableDepthTest = desc.m_EnableDepthTest;
            dsaDesc.m_DepthBufferReadOnly = desc.m_DepthBufferReadOnly;
            dsaDesc.m_DepthFunc = desc.m_DepthFunc;
            dsaDesc.m_EnableStencilTest = desc.m_EnableStencilTest;
            dsaDesc.m_StencilBufferReadOnly = desc.m_StencilBufferReadOnly;
            dsaDesc.m_StencilFunc = desc.m_StencilFunc;
            dsaDesc.m_StencilMask = desc.m_StencilMask;
            dsaDesc.m_StencilFail = desc.m_StencilFail;
            dsaDesc.m_StencilPassDepthFail = desc.m_StencilPassDepthFail;
            dsaDesc.m_StencilPass = desc.m_StencilPass;
            auto attachment = new D3D11DepthStencilAttachment(dsaDesc, m_Device, m_DeviceContext);
            m_DepthStencilAttachment = attachment;
            m_DepthStencilView = attachment->getDepthStencilView();
        }
    }

    D3D11FrameBuffer::~D3D11FrameBuffer() {
        // do not release render target views here, these objects are managed by the color attachments objects
        // same for the depth stencil view
    }

    void D3D11FrameBuffer::bind() {
        m_DeviceContext->PSSetShaderResources(0, m_RenderTargetViews.size(), &m_Nullable[0]);

        if (m_DepthStencilAttachment){
            m_DepthStencilAttachment->bind();
            m_DeviceContext->OMSetRenderTargets(m_RenderTargetViews.size(), &m_RenderTargetViews[0], m_DepthStencilView);
        }
        else{
            m_DeviceContext->OMSetRenderTargets(m_RenderTargetViews.size(), &m_RenderTargetViews[0], nullptr);
        }
        m_DeviceContext->RSSetViewports(1, &m_Viewport);
    }

    void D3D11FrameBuffer::unbind() {

    }

    void D3D11FrameBuffer::clear() {
        for (const auto renderTargetView : m_RenderTargetViews){
            m_DeviceContext->ClearRenderTargetView(renderTargetView, m_ClearColor);
        }
        if (m_DepthStencilAttachment){
            m_DepthStencilAttachment->clear();
        }
    }


    void D3D11FrameBuffer::setPosition(int32 xPos, int32 yPos) {
        m_XPos = xPos;
        m_YPos = yPos;
        m_Viewport.TopLeftX = static_cast<float>(m_XPos);
        m_Viewport.TopLeftY = static_cast<float>(m_YPos);
    }

    void D3D11FrameBuffer::setSize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        m_Viewport.Width = static_cast<float>(m_Width);
        m_Viewport.Height = static_cast<float>(m_Height);
        for (auto& colorAttachment : m_ColorAttachments){
            colorAttachment->onResize(width, height);
        }
    }


    D3D11DefaultFrameBuffer::D3D11DefaultFrameBuffer(const FrameBufferDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext, IDXGISwapChain *swapChain)
    : FrameBuffer(desc),
    m_Device(device),
    m_DeviceContext(deviceContext),
    m_SwapChain(swapChain),
    m_BackRenderTarget(nullptr),
    m_BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM){
        m_Viewport.Width = static_cast<float>(m_Width);
        m_Viewport.Height = static_cast<float>(m_Height);
        m_Viewport.MinDepth = desc.m_MinDepth;
        m_Viewport.MaxDepth = desc.m_MaxDepth;
        m_Viewport.TopLeftX = static_cast<float>(m_XPos);
        m_Viewport.TopLeftY = static_cast<float>(m_YPos);

        ID3D11Texture2D* backBuffer = nullptr;
        SR_D3D11_CALL(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
        if (backBuffer){
            SR_D3D11_CALL(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_BackRenderTarget));
            D3D11_TEXTURE2D_DESC backBufferDesc;
            backBuffer->GetDesc(&backBufferDesc);
            m_BackBufferFormat = backBufferDesc.Format;
            backBuffer->Release();

            if (desc.m_EnableDepthTest or desc.m_EnableStencilTest){
                DepthStencilAttachmentDesc dsaDesc;
                dsaDesc.m_Width = m_Width;
                dsaDesc.m_Height = m_Height;
                dsaDesc.m_EnableDepthTest = desc.m_EnableDepthTest;
                dsaDesc.m_DepthBufferReadOnly = desc.m_DepthBufferReadOnly;
                dsaDesc.m_DepthFunc = desc.m_DepthFunc;
                dsaDesc.m_EnableStencilTest = desc.m_EnableStencilTest;
                dsaDesc.m_StencilBufferReadOnly = desc.m_StencilBufferReadOnly;
                dsaDesc.m_StencilFunc = desc.m_StencilFunc;
                dsaDesc.m_StencilMask = desc.m_StencilMask;
                dsaDesc.m_StencilFail = desc.m_StencilFail;
                dsaDesc.m_StencilPassDepthFail = desc.m_StencilPassDepthFail;
                dsaDesc.m_StencilPass = desc.m_StencilPass;
                auto attachment = new D3D11DepthStencilAttachment(dsaDesc, m_Device, m_DeviceContext);
                m_DepthStencilAttachment = attachment;
                m_DepthStencilView = attachment->getDepthStencilView();
            }

        }
        else{
            SR_CORE_EXCEPTION("Failed to get back buffer from swap chain");
        }

    }

    D3D11DefaultFrameBuffer::~D3D11DefaultFrameBuffer() {
        if (m_BackRenderTarget){
            m_BackRenderTarget->Release();
        }
    }

    void D3D11DefaultFrameBuffer::bind() {
        if (m_DepthStencilAttachment){
            m_DepthStencilAttachment->bind();
            m_DeviceContext->OMSetRenderTargets(1, &m_BackRenderTarget, m_DepthStencilView);
        }
        else{
            m_DeviceContext->OMSetRenderTargets(1, &m_BackRenderTarget, nullptr);
        }
        m_DeviceContext->RSSetViewports(1, &m_Viewport);
    }

    void D3D11DefaultFrameBuffer::unbind() {

    }

    void D3D11DefaultFrameBuffer::clear() {
        m_DeviceContext->ClearRenderTargetView(m_BackRenderTarget, m_ClearColor);
        if (m_DepthStencilAttachment){
            m_DepthStencilAttachment->clear();
        }
    }

    void D3D11DefaultFrameBuffer::setPosition(int32 xPos, int32 yPos) {
        m_XPos = xPos;
        m_YPos = yPos;
        m_Viewport.TopLeftX = static_cast<float>(m_XPos);
        m_Viewport.TopLeftY = static_cast<float>(m_YPos);
    }

    void D3D11DefaultFrameBuffer::setSize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        m_Viewport.Width = static_cast<float>(m_Width);
        m_Viewport.Height = static_cast<float>(m_Height);

        if (m_BackRenderTarget){
            m_BackRenderTarget->Release();
        }

        SR_D3D11_CALL(m_SwapChain->ResizeBuffers(1, m_Width, m_Height, m_BackBufferFormat, 0));

        ID3D11Texture2D* backBuffer = nullptr;
        SR_D3D11_CALL(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
        if (backBuffer){
            SR_D3D11_CALL(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_BackRenderTarget));
            D3D11_TEXTURE2D_DESC backBufferDesc;
            backBuffer->GetDesc(&backBufferDesc);
            m_BackBufferFormat = backBufferDesc.Format;
            backBuffer->Release();

        }
        else{
            SR_CORE_EXCEPTION("Failed to get back buffer from swap chain");
        }
    }


}

#endif
