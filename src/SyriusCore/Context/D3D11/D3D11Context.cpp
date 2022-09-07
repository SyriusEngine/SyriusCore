#include "D3D11Context.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11Context::D3D11Context(HWND &hwnd)
    : Context(SR_API_D3D11),
    m_Hwnd(hwnd),
    m_DeviceContext(nullptr),
    m_Device(nullptr),
    m_SwapChain(nullptr){

        DXGI_SWAP_CHAIN_DESC scDesc = { 0 };
        scDesc.BufferDesc.Width = 0;
        scDesc.BufferDesc.Height = 0;
        scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;  // Is this the correct format, seems like R8G8B8A8 is the correct format?
        scDesc.BufferDesc.RefreshRate.Numerator = 0;
        scDesc.BufferDesc.RefreshRate.Denominator = 0;
        scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        scDesc.SampleDesc.Count = 1;
        scDesc.SampleDesc.Quality = 0;
        scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scDesc.BufferCount = 1; // backbuffer count
        scDesc.OutputWindow = m_Hwnd;
        scDesc.Windowed = TRUE;
        scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scDesc.Flags = 0;

        SR_D3D11_CALL(D3D11CreateDeviceAndSwapChain(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                D3D11_CREATE_DEVICE_DEBUG,
                nullptr,
                0,
                D3D11_SDK_VERSION,
                &scDesc,
                &m_SwapChain,
                &m_Device,
                nullptr,
                &m_DeviceContext
                ));

        m_BackgroundColor[0] = 2.0f;
        m_BackgroundColor[1] = 3.0f;
        m_BackgroundColor[2] = 8.0f;
        m_BackgroundColor[3] = 1.0f;

        ID3D11Resource* backBuffer = nullptr;
        SR_D3D11_CALL(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer)));
        if (backBuffer){
            SR_D3D11_CALL(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTarget));
            m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, nullptr);
            backBuffer->Release();
        }

        auto fbSize = getFramebufferSize();

        D3D11_VIEWPORT vp;
        vp.Width = fbSize.m_Width;
        vp.Height = fbSize.m_Height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        m_DeviceContext->RSSetViewports(1, &vp);

    }

    D3D11Context::~D3D11Context() {
        if (m_RenderTarget){
            m_RenderTarget->Release();
        }
        if (m_SwapChain) {
            m_SwapChain->Release();
        }
        if (m_DeviceContext) {
            m_DeviceContext->Release();
        }
        if (m_Device) {
            m_Device->Release();
        }
    }

    void D3D11Context::swapBuffers() {
        SR_CORE_PRECONDITION(m_SwapChain != nullptr, "SwapChain is nullptr");

        SR_D3D11_CHECK_DEVICE_REMOVED(m_SwapChain->Present(m_VerticalSync, 0), m_Device);
    }

    void D3D11Context::setVerticalSynchronisation(bool enable) {
        m_VerticalSync = enable;
    }

    void D3D11Context::createImGuiContext() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(m_Hwnd);

        ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
        m_ImGuiContext = ImGui::GetCurrentContext();
    }

    void D3D11Context::destroyImGuiContext() {
        ImGui::SetCurrentContext(m_ImGuiContext);

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        m_ImGuiContext = nullptr;
    }

    void D3D11Context::onImGuiBegin() {
        SR_CORE_PRECONDITION(m_ImGuiContext != nullptr, "ImGui context is nullptr");

        ImGui::SetCurrentContext(m_ImGuiContext);
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void D3D11Context::onImGuiEnd() {
        SR_CORE_PRECONDITION(m_ImGuiContext != nullptr, "ImGui context is nullptr");

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    FramebufferSize D3D11Context::getFramebufferSize() {
        RECT area;
        GetClientRect(m_Hwnd, &area);
        FramebufferSize size{};
        size.m_Width = area.right;
        size.m_Height = area.bottom;
        return size;
    }

    std::string D3D11Context::getAPIVersion() {
        return std::string();
    }

    std::string D3D11Context::getDeviceName() {
        return std::string();
    }

    std::string D3D11Context::getShadingLanguageVersion() {
        return std::string();
    }

    int32 D3D11Context::getMaxFramebufferWidth() {
        return 0;
    }

    int32 D3D11Context::getMaxFramebufferHeight() {
        return 0;
    }

    int32 D3D11Context::getMaxFramebufferTextureAttachments() {
        return 0;
    }

    int32 D3D11Context::getMaxTextureSlots() {
        return 0;
    }

    int32 D3D11Context::getMaxTexture2DSize() {
        return D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
    }

    int32 D3D11Context::getMaxConstantBufferSize() {
        return 0;
    }

    int32 D3D11Context::getMaxDepthBufferBits() {
        return 0;
    }

    FrameBuffer *D3D11Context::getDefaultFrameBuffer() {
        return nullptr;
    }

    void D3D11Context::setClearColor(float r, float g, float b, float a) {
        m_BackgroundColor[0] = r;
        m_BackgroundColor[1] = g;
        m_BackgroundColor[2] = b;
        m_BackgroundColor[3] = a;
    }

    void D3D11Context::clear() {
        m_DeviceContext->ClearRenderTargetView(m_RenderTarget, m_BackgroundColor);
    }

    void D3D11Context::draw(VertexArray *vao) {
        m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, nullptr);

        vao->drawBuffers();

    }

    void D3D11Context::drawInstanced(VertexArray *vao, uint32 instanceCount) {

    }

    void D3D11Context::onResize(uint32 width, uint32 height) {

    }

    ShaderModule *D3D11Context::createShaderModule(const ShaderModuleDesc &desc) {
        return new D3D11ShaderModule(desc, m_Device, m_DeviceContext);
    }

    Shader *D3D11Context::createShader(const ShaderDesc &desc) {
        return new D3D11Shader(desc, m_Device, m_DeviceContext);
    }

    VertexBuffer *D3D11Context::createVertexBuffer(const VertexBufferDesc &desc) {
        return new D3D11VertexBuffer(desc, m_Device, m_DeviceContext);
    }

    IndexBuffer *D3D11Context::createIndexBuffer(const IndexBufferDesc &desc) {
        return new D3D11IndexBuffer(desc, m_Device, m_DeviceContext);
    }

    VertexArray *D3D11Context::createVertexArray(const VertexArrayDesc &desc) {
        if (desc.m_IndexBuffer != nullptr) {
            return new D3D11VertexArrayIndexed(desc, m_Device, m_DeviceContext);
        }
        else {
            return new D3D11VertexArray(desc, m_Device, m_DeviceContext);
        }
    }

    ConstantBuffer *D3D11Context::createConstantBuffer(const ConstantBufferDesc &desc) {
        auto shaderStage = desc.m_ShaderStage;
        switch (shaderStage) {
            case SR_SHADER_VERTEX:      return new D3D11ConstantBufferVertex(desc, m_Device, m_DeviceContext);
            case SR_SHADER_FRAGMENT:    return new D3D11ConstantBufferPixel(desc, m_Device, m_DeviceContext);
            case SR_SHADER_GEOMETRY:    return new D3D11ConstantBufferGeometry(desc, m_Device, m_DeviceContext);
            default: {
                SR_CORE_EXCEPTION("Unsupported shader stage for constant buffer");
                return nullptr;
            }
        }
    }

    FrameBuffer *D3D11Context::createFrameBuffer(const FrameBufferDesc &desc) {
        return nullptr;
    }

    Texture2D *D3D11Context::createTexture2D(const Texture2DDesc& desc) {
        return nullptr;
    }
}

#endif
