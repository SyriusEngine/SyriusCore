#include "D3D11DepthStencilAttachment.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    DXGI_FORMAT getD3d11DepthStencilBufferFormat(SR_TEXTURE_FORMAT format){
        switch (format){
            case SR_TEXTURE_DEPTH_16: return DXGI_FORMAT_R16_TYPELESS;
            case SR_TEXTURE_DEPTH_24: return DXGI_FORMAT_R24G8_TYPELESS;
            case SR_TEXTURE_DEPTH_32: return DXGI_FORMAT_R32_TYPELESS;
            case SR_TEXTURE_DEPTH_24_STENCIL_8: return DXGI_FORMAT_R24G8_TYPELESS;
            case SR_TEXTURE_DEPTH_32_STENCIL_8: return DXGI_FORMAT_R32G8X24_TYPELESS;
            default: {
                SR_LOG_WARNING("D3D11DepthStencilAttachment", "Invalid depth stencil buffer format ({}) , defaulting to depth 24 stencil 8", format);
                return DXGI_FORMAT_R24G8_TYPELESS;
            }
        }
    }

    DXGI_FORMAT getD3d11DepthStencilViewFormat(SR_TEXTURE_FORMAT format){
        switch (format){
            case SR_TEXTURE_DEPTH_16: return DXGI_FORMAT_D16_UNORM;
            case SR_TEXTURE_DEPTH_24: return DXGI_FORMAT_D24_UNORM_S8_UINT;
            case SR_TEXTURE_DEPTH_32: return DXGI_FORMAT_D32_FLOAT;
            case SR_TEXTURE_DEPTH_24_STENCIL_8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
            case SR_TEXTURE_DEPTH_32_STENCIL_8: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
            default: {
                SR_LOG_WARNING("D3D11DepthStencilAttachment", "Invalid depth stencil view format ({}) , defaulting to depth 24 stencil 8", format);
                return DXGI_FORMAT_D24_UNORM_S8_UINT;
            }
        }
    }

    DXGI_FORMAT getD3d11DepthStencilShaderResourceViewFormat(SR_TEXTURE_FORMAT format){
        switch (format){
            case SR_TEXTURE_DEPTH_16: return DXGI_FORMAT_R16_UNORM;
            case SR_TEXTURE_DEPTH_24: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            case SR_TEXTURE_DEPTH_32: return DXGI_FORMAT_R32_FLOAT;
            case SR_TEXTURE_DEPTH_24_STENCIL_8: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            case SR_TEXTURE_DEPTH_32_STENCIL_8: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
            default: {
                SR_LOG_WARNING("D3D11DepthStencilAttachment", "Invalid depth stencil shader resource view format ({}) , defaulting to depth 24 stencil 8", format);
                return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            }
        }
    }

    D3D11DepthStencilAttachment::D3D11DepthStencilAttachment(const DepthStencilAttachmentDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *deviceContext):
    DepthStencilAttachment(desc, deviceLimits),
    m_Device(device),
    m_Context(deviceContext),
    m_Buffer(nullptr),
    m_State(nullptr),
    m_View(nullptr),
    m_ShaderResourceView(nullptr),
    m_ClearFlag(static_cast<D3D11_CLEAR_FLAG>(0)){
        setClearFlag();
        createState();
        createBufferAndView();
    }

    D3D11DepthStencilAttachment::~D3D11DepthStencilAttachment() {
        destroyBufferAndView();
        destroyState();
    }

    void D3D11DepthStencilAttachment::bind() {
        m_Context->OMSetDepthStencilState(m_State, 1);
    }

    void D3D11DepthStencilAttachment::unbind() {
        m_Context->OMSetDepthStencilState(nullptr, 1);
    }

    void D3D11DepthStencilAttachment::bindShaderResource(u32 slot) {
        SR_PRECONDITION(m_EnableShaderAccess, "Shader access is not enabled for this  depth stencil attachment");

        m_Context->PSSetShaderResources(slot, 1, &m_ShaderResourceView);
    }

    void D3D11DepthStencilAttachment::clear() {
        m_Context->ClearDepthStencilView(m_View, m_ClearFlag, m_ClearDepth, m_ClearStencil);
    }

    void D3D11DepthStencilAttachment::onResize(u32 width, u32 height) {
        destroyBufferAndView();

        m_Width = width;
        m_Height = height;

        setClearFlag();
        createBufferAndView();
    }

    UP<Image> D3D11DepthStencilAttachment::getData() {
        return {};
    }

    u64 D3D11DepthStencilAttachment::getIdentifier() const {
        return reinterpret_cast<u64>(m_State);
    }

    void D3D11DepthStencilAttachment::enableDepthTest(bool enable) {
        m_EnableDepthTest = enable;
        setClearFlag();
        createState();
    }

    void D3D11DepthStencilAttachment::setDepthFunc(SR_COMPARISON_FUNC func) {
        m_DepthFunc = func;
        createState();
    }

    void D3D11DepthStencilAttachment::setDepthMask(SR_DEPTH_MASK mask) {
        m_DepthMask = mask;
        createState();
    }

    ID3D11DepthStencilView *D3D11DepthStencilAttachment::getDepthStencilView() const {
        return m_View;
    }

    ID3D11DepthStencilState *D3D11DepthStencilAttachment::getDepthStencilState() const {
        return m_State;
    }

    void D3D11DepthStencilAttachment::setClearFlag() {
        u32 flags = 0;
        if (m_EnableDepthTest){
            flags |= D3D11_CLEAR_DEPTH;
        }
        if (m_EnableStencilTest){
            flags |= D3D11_CLEAR_STENCIL;
        }
        m_ClearFlag = static_cast<D3D11_CLEAR_FLAG>(flags);
    }

    void D3D11DepthStencilAttachment::createState() {
        if (m_State){
            m_State->Release();
        }

        D3D11_DEPTH_STENCIL_DESC stateDesc = { 0 };
        // Depth test parameters
        stateDesc.DepthEnable = m_EnableDepthTest;
        stateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        stateDesc.DepthFunc = getD3d11ComparisonFunc(m_DepthFunc);
        // Stencil test parameters
        stateDesc.StencilEnable = m_EnableStencilTest;
        stateDesc.StencilReadMask = m_StencilMask;
        stateDesc.StencilWriteMask = m_StencilMask;
        // Stencil operations if pixel is front-facing
        stateDesc.FrontFace.StencilFailOp = getD3d11StencilFunc(m_StencilFail);
        stateDesc.FrontFace.StencilDepthFailOp = getD3d11StencilFunc(m_StencilPassDepthFail);
        stateDesc.FrontFace.StencilPassOp = getD3d11StencilFunc(m_StencilPass);
        stateDesc.FrontFace.StencilFunc = getD3d11ComparisonFunc(m_StencilFunc);
        // Stencil operations if pixel is back-facing
        stateDesc.BackFace.StencilFailOp = getD3d11StencilFunc(m_StencilFail);
        stateDesc.BackFace.StencilDepthFailOp = getD3d11StencilFunc(m_StencilPassDepthFail);
        stateDesc.BackFace.StencilPassOp = getD3d11StencilFunc(m_StencilPass);
        stateDesc.BackFace.StencilFunc = getD3d11ComparisonFunc(m_StencilFunc);
        SR_CORE_D3D11_CALL(m_Device->CreateDepthStencilState(&stateDesc, &m_State));
    }

    void D3D11DepthStencilAttachment::createBufferAndView() {
        D3D11_TEXTURE2D_DESC bufferDesc;
        bufferDesc.Width = m_Width;
        bufferDesc.Height = m_Height;
        bufferDesc.MipLevels = 1;
        bufferDesc.ArraySize = 1;
        bufferDesc.Format = getD3d11DepthStencilBufferFormat(m_Format);
        bufferDesc.SampleDesc.Count = 1;
        bufferDesc.SampleDesc.Quality = 0;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        if (m_EnableShaderAccess){
            bufferDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
        }
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&bufferDesc, nullptr, &m_Buffer));

        D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
        viewDesc.Format = getD3d11DepthStencilViewFormat(m_Format);
        viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipSlice = 0;
        viewDesc.Flags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateDepthStencilView(m_Buffer, &viewDesc, &m_View));

        if (m_EnableShaderAccess){
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            srvDesc.Format = getD3d11DepthStencilShaderResourceViewFormat(m_Format);
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;
            srvDesc.Texture2D.MostDetailedMip = 0;
            SR_CORE_D3D11_CALL(m_Device->CreateShaderResourceView(m_Buffer, &srvDesc, &m_ShaderResourceView));
        }
    }

    void D3D11DepthStencilAttachment::destroyState() {
        if (m_State){
            m_State->Release();
            m_State = nullptr;
        }
    }

    void D3D11DepthStencilAttachment::destroyBufferAndView() {
        if (m_View) {
            m_View->Release();
            m_View = nullptr;
        }
        if (m_Buffer) {
            m_Buffer->Release();
            m_Buffer = nullptr;
        }
        if (m_ShaderResourceView) {
            m_ShaderResourceView->Release();
            m_ShaderResourceView = nullptr;
        }
    }
}

#endif
