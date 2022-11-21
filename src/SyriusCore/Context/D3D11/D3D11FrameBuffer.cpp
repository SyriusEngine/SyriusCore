#include "D3D11FrameBuffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11FrameBuffer::D3D11FrameBuffer(const FrameBufferDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext)
    : FrameBuffer(desc),
    m_Device(device),
    m_DeviceContext(deviceContext),
    m_Nullable(desc.m_ColorAttachments.size(), nullptr),
    m_DepthStencilView(nullptr){
        m_Viewport.Width = static_cast<float>(m_Width);
        m_Viewport.Height = static_cast<float>(m_Height);
        m_Viewport.MinDepth = desc.m_MinDepth;
        m_Viewport.MaxDepth = desc.m_MaxDepth;
        m_Viewport.TopLeftX = static_cast<float>(m_XPos);
        m_Viewport.TopLeftY = static_cast<float>(m_YPos);

        for (auto& caDesc: desc.m_ColorAttachments){
            auto attachment = new D3D11ColorAttachment(caDesc, m_Device, m_DeviceContext);
            m_ColorAttachments.push_back(attachment);
            m_RenderTargetViews.push_back(attachment->getRenderTargetView());
            m_Nullable.emplace_back(nullptr);
            m_NullableRenderTargetViews.emplace_back(nullptr);
        }

        if (desc.m_DepthStencilAttachment.m_EnableDepthTest or desc.m_DepthStencilAttachment.m_EnableStencilTest){
            auto attachment = new D3D11DepthStencilAttachment(desc.m_DepthStencilAttachment, m_Device, m_DeviceContext);
            m_DepthStencilAttachment = attachment;
            m_DepthStencilView = attachment->getDepthStencilView();
        }
    }

    D3D11FrameBuffer::~D3D11FrameBuffer() {
        // do not release render target views here, these objects are managed by the color attachments objects
        // same for the depth stencil view
    }

    void D3D11FrameBuffer::bind() {
        // unbind all shader inputs, a render target cannot be bound as a shader input if it is still bound as a texture
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
        m_DeviceContext->OMSetRenderTargets(m_RenderTargetViews.size(), &m_NullableRenderTargetViews[0], nullptr);

    }

    void D3D11FrameBuffer::clear() {
        for (const auto colorAttachment: m_ColorAttachments){
            colorAttachment->clear();
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
    m_DepthStencilView(nullptr){
        SR_CORE_PRECONDITION(m_SwapChain != nullptr, "Swap chain cannot be null");
        SR_CORE_PRECONDITION(m_Device != nullptr, "Device cannot be null");
        SR_CORE_PRECONDITION(m_DeviceContext != nullptr, "Device context cannot be null");
        SR_CORE_PRECONDITION(m_Width > 0, "Width must be greater than 0");
        SR_CORE_PRECONDITION(m_Height > 0, "Height must be greater than 0");
        SR_CORE_PRECONDITION(desc.m_ColorAttachments.size() == 1, "Default frame buffer must have exactly one color attachment");

        m_Viewport.Width = static_cast<float>(m_Width);
        m_Viewport.Height = static_cast<float>(m_Height);
        m_Viewport.MinDepth = desc.m_MinDepth;
        m_Viewport.MaxDepth = desc.m_MaxDepth;
        m_Viewport.TopLeftX = static_cast<float>(m_XPos);
        m_Viewport.TopLeftY = static_cast<float>(m_YPos);

        m_BackRenderTarget = new D3D11BackBufferColorAttachment(desc.m_ColorAttachments[0] , m_Device, m_DeviceContext, m_SwapChain);
        m_ColorAttachments.push_back(m_BackRenderTarget);
        m_BackRenderTargetView = m_BackRenderTarget->getRenderTargetView();
    }

    D3D11DefaultFrameBuffer::~D3D11DefaultFrameBuffer() {
        delete m_BackRenderTarget;
    }

    void D3D11DefaultFrameBuffer::bind() {
        if (m_DepthStencilAttachment){
            m_DepthStencilAttachment->bind();
            m_DeviceContext->OMSetRenderTargets(1, &m_BackRenderTargetView, m_DepthStencilView);
        }
        else{
            m_DeviceContext->OMSetRenderTargets(1, &m_BackRenderTargetView, nullptr);
        }
        m_DeviceContext->RSSetViewports(1, &m_Viewport);
    }

    void D3D11DefaultFrameBuffer::unbind() {

    }

    void D3D11DefaultFrameBuffer::clear() {
        m_BackRenderTarget->clear();
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

        m_BackRenderTarget->onResize(width, height);
        m_BackRenderTargetView = m_BackRenderTarget->getRenderTargetView();
    }
}

#endif
