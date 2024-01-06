#include "D3D11DepthStencilAttachment.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    DXGI_FORMAT getD3d11DepthStencilBufferFormat(SR_TEXTURE_FORMAT format){
        switch (format){
            case SR_TEXTURE_DEPTH_16: return DXGI_FORMAT_D16_UNORM;
            case SR_TEXTURE_DEPTH_24: return DXGI_FORMAT_D24_UNORM_S8_UINT;
            case SR_TEXTURE_DEPTH_32: return DXGI_FORMAT_D32_FLOAT;
            case SR_TEXTURE_DEPTH_24_STENCIL_8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
            case SR_TEXTURE_DEPTH_32_STENCIL_8: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
            default: {
                SR_CORE_WARNING("Invalid depth stencil buffer format, defaulting to depth 24 stencil 8");
                return DXGI_FORMAT_D24_UNORM_S8_UINT;
            }
        }
    }

    D3D11DepthStencilAttachment::D3D11DepthStencilAttachment(const DepthStencilAttachmentDesc &desc, ID3D11Device *device, ID3D11DeviceContext *deviceContext):
    DepthStencilAttachment(desc),
    m_Device(device),
    m_Context(deviceContext),
    m_DepthStencilBuffer(nullptr),
    m_DepthStencilState(nullptr),
    m_DepthStencilView(nullptr),
    m_D3DFormat(getD3d11DepthStencilBufferFormat(desc.format)){
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
        bufferDesc.Format = m_D3DFormat;
        bufferDesc.SampleDesc.Count = 1;
        bufferDesc.SampleDesc.Quality = 0;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&bufferDesc, nullptr, &m_DepthStencilBuffer));

        D3D11_DEPTH_STENCIL_DESC stateDesc = { 0 };
        // Depth test parameters
        stateDesc.DepthEnable = desc.enableDepthTest;
        stateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        stateDesc.DepthFunc = getD3d11ComparisonFunc(desc.depthFunc);
        // Stencil test parameters
        stateDesc.StencilEnable = desc.enableStencilTest;
        stateDesc.StencilReadMask = desc.stencilMask;
        stateDesc.StencilWriteMask = desc.stencilMask;
        // Stencil operations if pixel is front-facing
        stateDesc.FrontFace.StencilFailOp = getD3d11StencilFunc(desc.stencilFail);
        stateDesc.FrontFace.StencilDepthFailOp = getD3d11StencilFunc(desc.stencilPassDepthFail);
        stateDesc.FrontFace.StencilPassOp = getD3d11StencilFunc(desc.stencilPass);
        stateDesc.FrontFace.StencilFunc = getD3d11ComparisonFunc(desc.stencilFunc);
        // Stencil operations if pixel is back-facing
        stateDesc.BackFace.StencilFailOp = getD3d11StencilFunc(desc.stencilFail);;
        stateDesc.BackFace.StencilDepthFailOp = getD3d11StencilFunc(desc.stencilPassDepthFail);;
        stateDesc.BackFace.StencilPassOp = getD3d11StencilFunc(desc.stencilPass);;
        stateDesc.BackFace.StencilFunc = getD3d11ComparisonFunc(desc.stencilFunc);
        SR_CORE_D3D11_CALL(m_Device->CreateDepthStencilState(&stateDesc, &m_DepthStencilState));

        D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
        viewDesc.Format = m_D3DFormat;
        viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipSlice = 0;
        viewDesc.Flags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &viewDesc, &m_DepthStencilView));

    }

    D3D11DepthStencilAttachment::~D3D11DepthStencilAttachment() {
        if (m_DepthStencilBuffer){
            m_DepthStencilBuffer->Release();
        }
        if (m_DepthStencilState){
            m_DepthStencilState->Release();
        }
        if (m_DepthStencilView){
            m_DepthStencilView->Release();
        }
    }

    void D3D11DepthStencilAttachment::bind() {
        m_Context->OMSetDepthStencilState(m_DepthStencilState, 1);
    }

    void D3D11DepthStencilAttachment::unbind() {
        m_Context->OMSetDepthStencilState(nullptr, 1);
    }

    void D3D11DepthStencilAttachment::bindShaderResource(uint32 slot) {

    }

    void D3D11DepthStencilAttachment::clear() {
        m_Context->ClearDepthStencilView(m_DepthStencilView, m_ClearFlag, m_ClearDepth, m_ClearStencil);
    }

    void D3D11DepthStencilAttachment::setSize(uint32 width, uint32 height) {
        m_DepthStencilView->Release();
        m_DepthStencilBuffer->Release();

        m_Width = width;
        m_Height = height;

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
        bufferDesc.Format = m_D3DFormat;
        bufferDesc.SampleDesc.Count = 1;
        bufferDesc.SampleDesc.Quality = 0;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&bufferDesc, nullptr, &m_DepthStencilBuffer));

        D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
        viewDesc.Format = m_D3DFormat;
        viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipSlice = 0;
        viewDesc.Flags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &viewDesc, &m_DepthStencilView));
    }

    Resource<Image> D3D11DepthStencilAttachment::getData() {
        return Resource<Image>();
    }

    uint64 D3D11DepthStencilAttachment::getIdentifier() const {
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
