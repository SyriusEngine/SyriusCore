#include "D3D11FrameBuffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11FrameBuffer::D3D11FrameBuffer(const FrameBufferDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext)
    : FrameBuffer(desc),
    m_Device(device),
    m_DeviceContext(deviceContext){
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
        // unbind all shader inputs, a render target cannot be bound as a shader input if it is still bound as a texture
        std::vector<ID3D11ShaderResourceView*> nullSRV(m_RenderTargetViews.size(), nullptr);
        m_DeviceContext->PSSetShaderResources(0, m_RenderTargetViews.size(), &nullSRV[0]);

        m_DeviceContext->OMSetRenderTargets(m_RenderTargetViews.size(), &m_RenderTargetViews[0], nullptr);
        m_DeviceContext->RSSetViewports(1, &m_Viewport);
    }

    void D3D11FrameBuffer::unbind() {

    }

    void D3D11FrameBuffer::setClearColor(float red, float green, float blue, float alpha) {
        m_ClearColor[0] = red;
        m_ClearColor[1] = green;
        m_ClearColor[2] = blue;
        m_ClearColor[3] = alpha;
    }

    void D3D11FrameBuffer::setPosition(int32 xPos, int32 yPos) {
        m_XPos = xPos;
        m_YPos = yPos;
        m_Viewport.TopLeftX = static_cast<float>(m_XPos);
        m_Viewport.TopLeftY = static_cast<float>(m_YPos);
    }

    void D3D11FrameBuffer::setDepthFunc(SR_DEPTH_FUNC func) {

    }

    void D3D11FrameBuffer::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        m_Viewport.Width = static_cast<float>(m_Width);
        m_Viewport.Height = static_cast<float>(m_Height);
        for (auto& colorAttachment : m_ColorAttachments){
            colorAttachment->onResize(width, height);
        }
    }

    void D3D11FrameBuffer::enableDepthTest() {

    }

    void D3D11FrameBuffer::disableDepthTest() {

    }

    void D3D11FrameBuffer::clear() {
        for (const auto renderTargetView : m_RenderTargetViews){
            m_DeviceContext->ClearRenderTargetView(renderTargetView, m_ClearColor);
        }
    }

    D3D11DefaultFrameBuffer::D3D11DefaultFrameBuffer(const FrameBufferDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext, IDXGISwapChain *swapChain)
    : FrameBuffer(desc),
    m_Device(device),
    m_DeviceContext(deviceContext),
    m_SwapChain(swapChain),
    m_BackRenderTarget(nullptr){
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
            backBuffer->Release();

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
        m_DeviceContext->OMSetRenderTargets(1, &m_BackRenderTarget, nullptr);
        m_DeviceContext->RSSetViewports(1, &m_Viewport);
    }

    void D3D11DefaultFrameBuffer::unbind() {

    }

    void D3D11DefaultFrameBuffer::setClearColor(float red, float green, float blue, float alpha) {
        m_ClearColor[0] = red;
        m_ClearColor[1] = green;
        m_ClearColor[2] = blue;
        m_ClearColor[3] = alpha;
    }

    void D3D11DefaultFrameBuffer::setPosition(int32 xPos, int32 yPos) {
        m_XPos = xPos;
        m_YPos = yPos;
        m_Viewport.TopLeftX = static_cast<float>(m_XPos);
        m_Viewport.TopLeftY = static_cast<float>(m_YPos);
    }

    void D3D11DefaultFrameBuffer::setDepthFunc(SR_DEPTH_FUNC func) {

    }

    void D3D11DefaultFrameBuffer::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        m_Viewport.Width = static_cast<float>(m_Width);
        m_Viewport.Height = static_cast<float>(m_Height);
    }

    void D3D11DefaultFrameBuffer::enableDepthTest() {

    }

    void D3D11DefaultFrameBuffer::disableDepthTest() {

    }

    void D3D11DefaultFrameBuffer::clear() {
        m_DeviceContext->ClearRenderTargetView(m_BackRenderTarget, m_ClearColor);
    }
}

#endif
