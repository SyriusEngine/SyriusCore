#include "D3D11FrameBuffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11FrameBuffer::D3D11FrameBuffer(const FrameBufferDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext)
    : FrameBuffer(desc),
    m_Device(device),
    m_DeviceContext(deviceContext),
    m_Nullable(m_ColorAttachments.size(), nullptr){
        m_Viewport.Width = static_cast<float>(m_Width);
        m_Viewport.Height = static_cast<float>(m_Height);
        m_Viewport.MinDepth = desc.m_MinDepth;
        m_Viewport.MaxDepth = desc.m_MaxDepth;
        m_Viewport.TopLeftX = static_cast<float>(m_XPos);
        m_Viewport.TopLeftY = static_cast<float>(m_YPos);

        for (const auto& attachmentDesc: desc.m_ColorAttachments){
            auto attachment = new D3D11ColorAttachment(attachmentDesc, m_Device, m_DeviceContext);
            m_ColorAttachments.push_back(attachment);
            m_RenderTargetViews.push_back(attachment->getRenderTargetView());
            m_Nullable.emplace_back(nullptr);
        }
    }

    D3D11FrameBuffer::~D3D11FrameBuffer() {
        // do not release render target views here, these objects are managed by the color attachments objects
    }

    void D3D11FrameBuffer::bind() {
        m_DeviceContext->PSSetShaderResources(0, m_RenderTargetViews.size(), &m_Nullable[0]);

        m_DeviceContext->OMSetRenderTargets(m_RenderTargetViews.size(), &m_RenderTargetViews[0], nullptr);
        m_DeviceContext->RSSetViewports(1, &m_Viewport);
    }

    void D3D11FrameBuffer::unbind() {

    }

    void D3D11FrameBuffer::clear() {
        bind();
        for (const auto renderTargetView : m_RenderTargetViews){
            m_DeviceContext->ClearRenderTargetView(renderTargetView, m_ClearColor);
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

    void D3D11FrameBuffer::setDepthFunc(SR_COMPARISON_FUNC func) {
        m_DepthFunc = func;
    }

    void D3D11FrameBuffer::setStencilFunc(SR_COMPARISON_FUNC func) {
        m_StencilFunc = func;
    }


    D3D11DefaultFrameBuffer::D3D11DefaultFrameBuffer(const FrameBufferDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext, IDXGISwapChain *swapChain)
    : FrameBuffer(desc),
    m_Device(device),
    m_DeviceContext(deviceContext),
    m_SwapChain(swapChain),
    m_BackRenderTarget(nullptr),
    m_BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM),
    m_DepthStencil(nullptr),
    m_DepthStencilResource(nullptr),
    m_DepthStencilState(nullptr){
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

            if (desc.m_EnableDepthTest){
                D3D11_TEXTURE2D_DESC descDepth;
                descDepth.Width = m_Width;
                descDepth.Height = m_Height;
                descDepth.MipLevels = 1;
                descDepth.ArraySize = 1;
                descDepth.Format = m_BackBufferFormat;
                descDepth.SampleDesc.Count = 1;
                descDepth.SampleDesc.Quality = 0;
                descDepth.Usage = D3D11_USAGE_DEFAULT;
                descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
                descDepth.CPUAccessFlags = 0;
                descDepth.MiscFlags = 0;
                SR_D3D11_CALL(m_Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencilResource));

                D3D11_DEPTH_STENCIL_DESC dsDesc;

                // Depth test parameters
                dsDesc.DepthEnable = true;
                dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                dsDesc.DepthFunc = getD3d11ComparisonFunc(desc.m_DepthFunc);
//                // Stencil test parameters
//                dsDesc.StencilEnable = true;
//                dsDesc.StencilReadMask = 0xFF;
//                dsDesc.StencilWriteMask = 0xFF;
//                // Stencil operations if pixel is front-facing
//                dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//                dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
//                dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//                dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//                // Stencil operations if pixel is back-facing
//                dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//                dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
//                dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//                dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
                SR_D3D11_CALL(m_Device->CreateDepthStencilState(&dsDesc, &m_DepthStencilState));

                D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
                descDSV.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
                descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                descDSV.Texture2D.MipSlice = 0;
                SR_D3D11_CALL(m_Device->CreateDepthStencilView(m_DepthStencilResource, &descDSV, &m_DepthStencil));

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
        if (m_EnableDepthTest){
            m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
        }
        m_DeviceContext->OMSetRenderTargets(1, &m_BackRenderTarget, m_DepthStencil);
        m_DeviceContext->RSSetViewports(1, &m_Viewport);
    }

    void D3D11DefaultFrameBuffer::unbind() {

    }

    void D3D11DefaultFrameBuffer::clear() {
        bind();
        m_DeviceContext->ClearRenderTargetView(m_BackRenderTarget, m_ClearColor);
    }

    void D3D11DefaultFrameBuffer::setPosition(int32 xPos, int32 yPos) {
        m_XPos = xPos;
        m_YPos = yPos;
        m_Viewport.TopLeftX = static_cast<float>(m_XPos);
        m_Viewport.TopLeftY = static_cast<float>(m_YPos);
    }

    void D3D11DefaultFrameBuffer::setSize(uint32 width, uint32 height) {
        bind();
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

    void D3D11DefaultFrameBuffer::setDepthFunc(SR_COMPARISON_FUNC func) {
        m_DepthFunc = func;
    }

    void D3D11DefaultFrameBuffer::setStencilFunc(SR_COMPARISON_FUNC func) {
        m_StencilFunc = func;
    }


}

#endif
