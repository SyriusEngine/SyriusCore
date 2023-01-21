#include "D3D11FrameBuffer.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11FrameBuffer::D3D11FrameBuffer(const ResourceView<FrameBufferDescription> &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext):
    FrameBuffer(desc),
    m_Device(device),
    m_DeviceContext(deviceContext),
    m_RenderTargetViews(),
    m_NullableRenderTargetViews(),
    m_NullableShaderResourceViews(){
        for (const auto& viewDesc: desc->getViewportDesc()){
            auto viewport = new D3D11Viewport(viewDesc, m_Device, m_DeviceContext);
            m_Viewports.emplace_back(viewport);
        }

        for (auto& caDesc: desc->getColorAttachmentDesc()){
            auto attachment = new D3D11ColorAttachment(caDesc, m_Device, m_DeviceContext);
            m_ColorAttachments.emplace_back(attachment);
            m_RenderTargetViews.push_back(attachment->getRenderTargetView());
            m_NullableShaderResourceViews.emplace_back(nullptr);
            m_NullableRenderTargetViews.emplace_back(nullptr);
        }

        if (!desc->getDepthStencilAttachmentDesc().empty()){
            auto attachment = new D3D11DepthStencilAttachment(desc->getDepthStencilAttachmentDesc().back(), m_Device, m_DeviceContext);
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(attachment);
            m_DepthStencilView = attachment->getDepthStencilView();
        }
    }

    D3D11FrameBuffer::~D3D11FrameBuffer() {

    }

    void D3D11FrameBuffer::bind() {
// unbind all shader inputs, a render target cannot be bound as a shader input if it is still bound as a texture
        m_DeviceContext->PSSetShaderResources(0, m_RenderTargetViews.size(), &m_NullableShaderResourceViews[0]);

        for (const auto& viewport: m_Viewports){
            viewport->bind();
        }
        if (m_DepthStencilAttachment.isValid()){
            m_DepthStencilAttachment->bind();
            m_DeviceContext->OMSetRenderTargets(m_RenderTargetViews.size(), &m_RenderTargetViews[0], m_DepthStencilView);
        }
        else{
            m_DeviceContext->OMSetRenderTargets(m_RenderTargetViews.size(), &m_RenderTargetViews[0], nullptr);
        }
    }

    void D3D11FrameBuffer::unbind() {
        m_DeviceContext->OMSetRenderTargets(m_RenderTargetViews.size(), &m_NullableRenderTargetViews[0], nullptr);
    }

    D3D11DefaultFrameBuffer::D3D11DefaultFrameBuffer(const ResourceView<FrameBufferDescription> &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext, IDXGISwapChain *swapChain):
    FrameBuffer(desc),
    m_Device(device),
    m_DeviceContext(deviceContext),
    m_SwapChain(swapChain),
    m_RenderTargetView(nullptr),
    m_DepthStencilView(nullptr){
        for (const auto& viewDesc: desc->getViewportDesc()){
            auto viewport = new D3D11Viewport(viewDesc, m_Device, m_DeviceContext);
            m_Viewports.emplace_back(viewport);
        }

        m_ColorAttachment = new D3D11DefaultColorAttachment(desc->getColorAttachmentDesc().back(), m_Device, m_DeviceContext, m_SwapChain);
        m_ColorAttachments.emplace_back(m_ColorAttachment);
        m_RenderTargetView = m_ColorAttachment->getRenderTargetView();

        if (!desc->getDepthStencilAttachmentDesc().empty()){
            auto attachment = new D3D11DepthStencilAttachment(desc->getDepthStencilAttachmentDesc().back(), m_Device, m_DeviceContext);
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(attachment);
            m_DepthStencilView = attachment->getDepthStencilView();
        }

    }

    D3D11DefaultFrameBuffer::~D3D11DefaultFrameBuffer() {

    }

    void D3D11DefaultFrameBuffer::bind() {
        for (const auto& viewport: m_Viewports){
            viewport->bind();
        }

        if (m_DepthStencilAttachment.isValid()){
            m_DepthStencilAttachment->bind();
            m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
        }
        else{
            m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);
        }

    }

    void D3D11DefaultFrameBuffer::unbind() {

    }

}

#endif
