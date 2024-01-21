#include "D3D11FrameBuffer.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11FrameBuffer::D3D11FrameBuffer(const ResourceView<FrameBufferDescription> &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *deviceContext):
    FrameBuffer(desc, deviceLimits),
    m_Device(device),
    m_DeviceContext(deviceContext),
    m_RenderTargetViews(),
    m_NullableRenderTargetViews(),
    m_NullableShaderResourceViews(),
    m_DepthStencilView(nullptr),
    m_D3D11DepthStencilAttachment(nullptr){
        for (const auto& viewDesc: desc->getViewportDesc()){
            auto viewport = new D3D11Viewport(viewDesc, m_DeviceLimits, m_Device, m_DeviceContext);
            m_Viewports.emplace_back(viewport);
        }

        for (auto& caDesc: desc->getColorAttachmentDesc()){
            auto attachment = new D3D11ColorAttachment(caDesc, m_DeviceLimits, m_Device, m_DeviceContext);
            m_ColorAttachments.emplace_back(attachment);
            m_D3D11ColorAttachments.emplace_back(attachment);
            m_RenderTargetViews.push_back(attachment->getRenderTargetView());
            m_NullableShaderResourceViews.emplace_back(nullptr);
            m_NullableRenderTargetViews.emplace_back(nullptr);
        }

        if (!desc->getDepthStencilAttachmentDesc().empty()){
            m_D3D11DepthStencilAttachment = new D3D11DepthStencilAttachment(desc->getDepthStencilAttachmentDesc().back(), m_DeviceLimits, m_Device, m_DeviceContext);
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(m_D3D11DepthStencilAttachment);
            m_DepthStencilView = m_D3D11DepthStencilAttachment->getDepthStencilView();
        }
        else{
            // we create the attachment anyway, but we make it such that both depth testing and stencil testing are disabled
            DepthStencilAttachmentDesc dummyDesc;
            dummyDesc.width = desc->getViewportDesc().begin()->width; // just use the first viewport's dimensions
            dummyDesc.height = desc->getViewportDesc().begin()->height;
            dummyDesc.enableDepthTest = false;
            dummyDesc.enableStencilTest = false;
            m_D3D11DepthStencilAttachment = new D3D11DepthStencilAttachment(dummyDesc, m_DeviceLimits, m_Device, m_DeviceContext);
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(m_D3D11DepthStencilAttachment);
            m_DepthStencilView = m_D3D11DepthStencilAttachment->getDepthStencilView();
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
        m_DepthStencilAttachment->bind();
        m_DeviceContext->OMSetRenderTargets(m_RenderTargetViews.size(), &m_RenderTargetViews[0], m_DepthStencilView);
    }

    void D3D11FrameBuffer::unbind() {
        m_DeviceContext->OMSetRenderTargets(m_RenderTargetViews.size(), &m_NullableRenderTargetViews[0], nullptr);
    }

    void D3D11FrameBuffer::onResize(uint32 width, uint32 height) {
        unbind();
        for (auto& viewport : m_Viewports){
            viewport->onResize(width, height);
        }
        /*
         * Only reason that this function is overridden, after resizing the color attachments, we need to
         * fetch the new render target views
         */
        m_RenderTargetViews.clear();
        for (auto& colorAttachment : m_D3D11ColorAttachments){
            colorAttachment->onResize(width, height);
            m_RenderTargetViews.push_back(colorAttachment->getRenderTargetView());
        }
        m_DepthStencilAttachment->onResize(width, height);
        m_DepthStencilView = m_D3D11DepthStencilAttachment->getDepthStencilView();
    }

    D3D11DefaultFrameBuffer::D3D11DefaultFrameBuffer(const ResourceView<FrameBufferDescription> &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *deviceContext, IDXGISwapChain *swapChain):
    FrameBuffer(desc, deviceLimits),
    m_Device(device),
    m_DeviceContext(deviceContext),
    m_SwapChain(swapChain),
    m_RenderTargetView(nullptr),
    m_DepthStencilView(nullptr),
    m_D3D11DepthStencilAttachment(nullptr){
        for (const auto& viewDesc: desc->getViewportDesc()){
            auto viewport = new D3D11Viewport(viewDesc, m_DeviceLimits, m_Device, m_DeviceContext);
            m_Viewports.emplace_back(viewport);
        }

        m_ColorAttachment = new D3D11DefaultColorAttachment(desc->getColorAttachmentDesc().back(), m_DeviceLimits, m_Device, m_DeviceContext, m_SwapChain);
        m_ColorAttachments.emplace_back(m_ColorAttachment);
        m_RenderTargetView = m_ColorAttachment->getRenderTargetView();

        if (!desc->getDepthStencilAttachmentDesc().empty()){
            m_D3D11DepthStencilAttachment = new D3D11DepthStencilAttachment(desc->getDepthStencilAttachmentDesc().back(), m_DeviceLimits, m_Device, m_DeviceContext);
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(m_D3D11DepthStencilAttachment);
            m_DepthStencilView = m_D3D11DepthStencilAttachment->getDepthStencilView();
        }
        else{
            // we create the attachment anyway, but we make it such that both depth testing and stencil testing are disabled
            DepthStencilAttachmentDesc dummyDesc;
            dummyDesc.width = m_ColorAttachment->getWidth(); // use the dimensions of the color attachment, which is created by the swap chain and my differ from the window/context dimensions
            dummyDesc.height = m_ColorAttachment->getHeight();
            dummyDesc.enableDepthTest = false;
            dummyDesc.enableStencilTest = false;
            m_D3D11DepthStencilAttachment = new D3D11DepthStencilAttachment(dummyDesc, m_DeviceLimits, m_Device, m_DeviceContext);
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(m_D3D11DepthStencilAttachment);
            m_DepthStencilView = m_D3D11DepthStencilAttachment->getDepthStencilView();
        }

    }

    D3D11DefaultFrameBuffer::~D3D11DefaultFrameBuffer() {

    }

    void D3D11DefaultFrameBuffer::bind() {
        for (const auto& viewport: m_Viewports){
            viewport->bind();
        }

        m_DepthStencilAttachment->bind();
        m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
    }

    void D3D11DefaultFrameBuffer::unbind() {

    }

    void D3D11DefaultFrameBuffer::onResize(uint32 width, uint32 height) {
        for (auto& viewport : m_Viewports){
            viewport->onResize(width, height);
        }
        /*
         * Only reason that this function is overridden, after resizing the color attachments, we need to
         * fetch the new render target views
         */
        m_ColorAttachment->onResize(width, height);
        m_RenderTargetView = m_ColorAttachment->getRenderTargetView();

        m_DepthStencilAttachment->onResize(width, height);
        m_DepthStencilView = m_D3D11DepthStencilAttachment->getDepthStencilView();
    }

}

#endif
