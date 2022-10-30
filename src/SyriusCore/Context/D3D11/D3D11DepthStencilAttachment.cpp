#include "D3D11DepthStencilAttachment.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11DepthStencilAttachment::D3D11DepthStencilAttachment(const DepthStencilAttachmentDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext)
    : DepthStencilAttachment(desc),
      m_Device(device),
      m_Context(deviceContext),
      m_DepthStencilBuffer(nullptr),
      m_DepthStencilState(nullptr),
      m_BufferView(nullptr),
      m_DepthStencilView(nullptr){
        uint32 flags = 0;
        if (m_EnableDepthTest){
            flags |= D3D11_CLEAR_DEPTH;
        }
        if (m_EnableStencilTest){
            flags |= D3D11_CLEAR_STENCIL;
        }
        m_ClearFlag = static_cast<D3D11_CLEAR_FLAG>(flags);

        D3D11_TEXTURE2D_DESC bufferDesc;
        bufferDesc.Width = m_Width;
        bufferDesc.Height = m_Height;
        bufferDesc.MipLevels = 1;
        bufferDesc.ArraySize = 1;
        bufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        bufferDesc.SampleDesc.Count = 1;
        bufferDesc.SampleDesc.Quality = 0;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        SR_D3D11_CALL(m_Device->CreateTexture2D(&bufferDesc, nullptr, &m_DepthStencilBuffer));

        D3D11_DEPTH_STENCIL_DESC stateDesc;
        // Depth test parameters
        stateDesc.DepthEnable = desc.m_EnableDepthTest;
        stateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        stateDesc.DepthFunc = getD3d11ComparisonFunc(desc.m_DepthFunc);
        // Stencil test parameters
        stateDesc.StencilEnable = desc.m_EnableStencilTest;
        stateDesc.StencilReadMask = desc.m_StencilMask;
        stateDesc.StencilWriteMask = desc.m_StencilMask;
        // Stencil operations if pixel is front-facing
        stateDesc.FrontFace.StencilFailOp = getD3d11StencilFunc(desc.m_StencilFail);
        stateDesc.FrontFace.StencilDepthFailOp = getD3d11StencilFunc(desc.m_StencilPassDepthFail);
        stateDesc.FrontFace.StencilPassOp = getD3d11StencilFunc(desc.m_StencilPass);
        stateDesc.FrontFace.StencilFunc = getD3d11ComparisonFunc(desc.m_StencilFunc);
        // Stencil operations if pixel is back-facing
        stateDesc.BackFace.StencilFailOp = getD3d11StencilFunc(desc.m_StencilFail);;
        stateDesc.BackFace.StencilDepthFailOp = getD3d11StencilFunc(desc.m_StencilPassDepthFail);;
        stateDesc.BackFace.StencilPassOp = getD3d11StencilFunc(desc.m_StencilPass);;
        stateDesc.BackFace.StencilFunc = getD3d11ComparisonFunc(desc.m_StencilFunc);
        SR_D3D11_CALL(m_Device->CreateDepthStencilState(&stateDesc, &m_DepthStencilState));

        D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
        viewDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipSlice = 0;
        SR_D3D11_CALL(m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &viewDesc, &m_DepthStencilView));

        D3D11_SHADER_RESOURCE_VIEW_DESC bufferViewDesc = {  };
        bufferViewDesc.Format = DXGI_FORMAT_R32_TYPELESS;
        bufferViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        bufferViewDesc.Texture2D.MipLevels = 1;
        bufferViewDesc.Texture2D.MostDetailedMip = 0;

        SR_D3D11_CALL(m_Device->CreateShaderResourceView(m_DepthStencilBuffer, &bufferViewDesc, &m_BufferView));

    }

    D3D11DepthStencilAttachment::~D3D11DepthStencilAttachment() {
        if (m_DepthStencilBuffer){
            m_DepthStencilBuffer->Release();
        }
        if (m_DepthStencilState){
            m_DepthStencilState->Release();
        }
        if (m_BufferView){
            m_BufferView->Release();
        }
        if (m_DepthStencilView){
            m_DepthStencilView->Release();
        }
    }

    void D3D11DepthStencilAttachment::bind() {
        m_Context->OMSetDepthStencilState(m_DepthStencilState, 1);
    }

    void D3D11DepthStencilAttachment::bindAsTexture(uint32 slot) {
        m_Context->PSSetShaderResources(slot, 1, &m_BufferView);
    }

    void D3D11DepthStencilAttachment::unbind() {
        m_Context->OMSetDepthStencilState(nullptr, 1);
    }

    void D3D11DepthStencilAttachment::clear() {
        m_Context->ClearDepthStencilView(m_DepthStencilView, m_ClearFlag, m_ClearDepth, m_ClearStencil);
    }

    void D3D11DepthStencilAttachment::onResize(uint32 width, uint32 height) {

    }

    void D3D11DepthStencilAttachment::setDepthFunc(SR_COMPARISON_FUNC func) {

    }

    void D3D11DepthStencilAttachment::setDepthBufferReadOnly(bool readOnly) {

    }

    void D3D11DepthStencilAttachment::setStencilFunc(SR_COMPARISON_FUNC func) {

    }

    void D3D11DepthStencilAttachment::setStencilBufferReadOnly(bool readOnly) {

    }

    void D3D11DepthStencilAttachment::setStencilMask(uint32 mask) {

    }

    uint64 D3D11DepthStencilAttachment::getIdentifier() {
        return reinterpret_cast<uint64>(m_DepthStencilState);
    }

    ID3D11DepthStencilView *D3D11DepthStencilAttachment::getDepthStencilView() const {
        return m_DepthStencilView;
    }

    ID3D11DepthStencilState *D3D11DepthStencilAttachment::getDepthStencilState() const {
        return m_DepthStencilState;
    }

}

#endif
