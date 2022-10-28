#include "D3D11Context.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11Context::D3D11Context(HWND &hwnd, const ContextDesc& desc)
    : Context(desc),
    m_Hwnd(hwnd),
    m_DeviceContext(nullptr),
    m_Device(nullptr),
    m_SwapChain(nullptr){

        DXGI_SWAP_CHAIN_DESC scDesc = { 0 };
        scDesc.BufferDesc.Width = 0;
        scDesc.BufferDesc.Height = 0;
        scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;  // DWM uses BGRA format, this is supposed to be slightly faster than RGBA
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
                &m_D3DVersion,
                &m_DeviceContext
                ));


        m_DefaultFrameBuffer = new D3D11DefaultFrameBuffer(desc.m_DefaultFrameBufferDesc, m_Device, m_DeviceContext, m_SwapChain);


    }

    D3D11Context::~D3D11Context() {
        delete m_DefaultFrameBuffer;
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

        SR_DXGI_GET_MESSAGES();

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
        switch (m_D3DVersion) {
            case D3D_FEATURE_LEVEL_9_1:         return "D3D_FEATURE_LEVEL_9_1";
            case D3D_FEATURE_LEVEL_9_2:         return "D3D_FEATURE_LEVEL_9_2";
            case D3D_FEATURE_LEVEL_9_3:         return "D3D_FEATURE_LEVEL_9_3";
            case D3D_FEATURE_LEVEL_10_0:        return "D3D_FEATURE_LEVEL_10_0";
            case D3D_FEATURE_LEVEL_10_1:        return "D3D_FEATURE_LEVEL_10_1";
            case D3D_FEATURE_LEVEL_11_0:        return "D3D_FEATURE_LEVEL_11_0";
            case D3D_FEATURE_LEVEL_11_1:        return "D3D_FEATURE_LEVEL_11_1";
            case D3D_FEATURE_LEVEL_12_0:        return "D3D_FEATURE_LEVEL_12_0";
            case D3D_FEATURE_LEVEL_12_1:        return "D3D_FEATURE_LEVEL_12_1";
            default:                            return "Unknown";
        }
    }

    std::string D3D11Context::getDeviceName() {
        return {};
    }

    std::string D3D11Context::getShadingLanguageVersion() {
        return "HLSL 5.0";
    }

    int32 D3D11Context::getMaxFramebufferWidth() {
        return 16384;
    }

    int32 D3D11Context::getMaxFramebufferHeight() {
        return 16384;
    }

    int32 D3D11Context::getMaxFramebufferTextureAttachments() {
        return 8;
    }

    int32 D3D11Context::getMaxTextureSlots() {
        return D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT ;
    }

    int32 D3D11Context::getMaxTexture2DSize() {
        return D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
    }

    int32 D3D11Context::getMaxConstantBufferSize() {
        return 65536;
    }

    int32 D3D11Context::getMaxDepthBufferBits() {
        return 32;
    }

    ShaderModule *D3D11Context::createShaderModule(const ShaderModuleDesc &desc) {
        auto ptr = new D3D11ShaderModule(desc, m_Device, m_DeviceContext);
        m_ShaderModules.push_back(ptr);
        return ptr;
    }

    Shader *D3D11Context::createShader(const ShaderDesc &desc) {
        auto ptr = new D3D11Shader(desc, m_Device, m_DeviceContext);
        m_Shaders.push_back(ptr);
        return ptr;
    }

    VertexBuffer *D3D11Context::createVertexBuffer(const VertexBufferDesc &desc) {
        auto ptr = new D3D11VertexBuffer(desc, m_Device, m_DeviceContext);
        m_VertexBuffers.push_back(ptr);
        return ptr;
    }

    IndexBuffer *D3D11Context::createIndexBuffer(const IndexBufferDesc &desc) {
        auto ptr = new D3D11IndexBuffer(desc, m_Device, m_DeviceContext);
        m_IndexBuffers.push_back(ptr);
        return ptr;
    }

    VertexArray *D3D11Context::createVertexArray(const VertexArrayDesc &desc) {
        VertexArray* ptr;
        if (desc.m_IndexBuffer != nullptr) {
            ptr =  new D3D11VertexArrayIndexed(desc, m_Device, m_DeviceContext);
        }
        else {
            ptr =  new D3D11VertexArray(desc, m_Device, m_DeviceContext);
        }
        m_VertexArrays.push_back(ptr);
        return ptr;
    }

    ConstantBuffer *D3D11Context::createConstantBuffer(const ConstantBufferDesc &desc) {
        ConstantBuffer* ptr;
        auto shaderStage = desc.m_ShaderStage;
        switch (shaderStage) {
            case SR_SHADER_VERTEX:      ptr = new D3D11ConstantBufferVertex(desc, m_Device, m_DeviceContext); break;
            case SR_SHADER_FRAGMENT:    ptr = new D3D11ConstantBufferPixel(desc, m_Device, m_DeviceContext); break;
            case SR_SHADER_GEOMETRY:    ptr = new D3D11ConstantBufferGeometry(desc, m_Device, m_DeviceContext); break;
            default: {
                SR_CORE_EXCEPTION("Unsupported shader stage for constant buffer");
                return nullptr;
            }
        }
        m_ConstantBuffers.push_back(ptr);
        return ptr;
    }

    FrameBuffer *D3D11Context::createFrameBuffer(const FrameBufferDesc &desc) {
        auto ptr = new D3D11FrameBuffer(desc, m_Device, m_DeviceContext);
        m_FrameBuffers.push_back(ptr);
        return ptr;
    }

    Texture2D *D3D11Context::createTexture2D(const Texture2DDesc& desc) {
        auto ptr = new D3D11Texture2D(desc, m_Device, m_DeviceContext);
        m_Textures2D.push_back(ptr);
        return ptr;
    }

    Sampler2D *D3D11Context::createSampler2D(const Sampler2DDesc &desc) {
        return nullptr;
    }
}

#endif
