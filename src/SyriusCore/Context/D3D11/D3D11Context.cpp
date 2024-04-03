#include "D3D11Context.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

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
        scDesc.BufferCount = 2; // backbuffer count, is equal to 2 because of DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL
        scDesc.OutputWindow = m_Hwnd;
        scDesc.Windowed = TRUE;
        scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allows toggling between fullscreen mode and windowed mode

        SR_CORE_D3D11_CALL(D3D11CreateDeviceAndSwapChain(
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


        auto defaultFbDesc = createFrameBufferLayout();

        ViewportDesc viewportDesc;
        viewportDesc.width = desc.backBufferWidth;
        viewportDesc.height = desc.backBufferHeight;
        defaultFbDesc->addViewportDesc(viewportDesc);

        ColorAttachmentDesc colorAttachmentDesc;
        colorAttachmentDesc.width = desc.backBufferWidth;
        colorAttachmentDesc.height = desc.backBufferHeight;
        defaultFbDesc->addColorAttachmentDesc(colorAttachmentDesc);

        if (desc.enableDepthTest or desc.enableStencilTest){
            DepthStencilAttachmentDesc dsaDesc;
            dsaDesc.width = desc.backBufferWidth;
            dsaDesc.height = desc.backBufferHeight;
            dsaDesc.enableDepthTest = desc.enableDepthTest;
            dsaDesc.enableStencilTest = desc.enableStencilTest;
            defaultFbDesc->addDepthStencilAttachmentDesc(dsaDesc);
        }

        m_DeviceLimits = createResource<D3D11DeviceLimits>(m_Device, m_DeviceContext, m_D3DVersion);

        auto ptr = new D3D11DefaultFrameBuffer(defaultFbDesc, m_DeviceLimits, m_Device, m_DeviceContext, m_SwapChain);
        m_FrameBuffers.emplace_back(ptr);

        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerDesc.CullMode = D3D11_CULL_NONE;
        rasterizerDesc.FrontCounterClockwise = false;
        rasterizerDesc.DepthBias = 0;
        rasterizerDesc.DepthBiasClamp = 0.0f;
        rasterizerDesc.SlopeScaledDepthBias = 0.0f;
        rasterizerDesc.DepthClipEnable = true;
        rasterizerDesc.ScissorEnable = false;
        rasterizerDesc.MultisampleEnable = false;
        rasterizerDesc.AntialiasedLineEnable = false;

        SR_CORE_D3D11_CALL(m_Device->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState));
        m_DeviceContext->RSSetState(m_RasterizerState);

        SR_CORE_POSTCONDITION(m_Device != nullptr, "DeviceLimits is null");
        SR_CORE_POSTCONDITION(m_DeviceContext != nullptr, "DeviceContext is null");
        SR_CORE_POSTCONDITION(m_SwapChain != nullptr, "SwapChain is null");
        SR_CORE_POSTCONDITION(m_DeviceLimits.get() != nullptr, "DeviceLimits is null");
        SR_CORE_POSTCONDITION(m_FrameBuffers.size() > 0, "Default framebuffer not present");
    }

    D3D11Context::~D3D11Context() {
        // every D3D11 object should be released before the device, device context and swap chain are released
        m_ShaderModules.clear();
        m_Shaders.clear();
        m_VertexBuffers.clear();
        m_IndexBuffers.clear();
        m_VertexDescriptions.clear();
        m_Textures2D.clear();
        m_Samplers.clear();
        m_ConstantBuffers.clear();
        m_VertexArrays.clear();
        m_FrameBuffers.clear();
        m_FrameBufferDescriptions.clear();
        m_Cubemaps.clear();

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

        SR_CORE_DXGI_GET_MESSAGES();

        SR_CORE_D3D11_CHECK_DEVICE_REMOVED(m_SwapChain->Present(m_VerticalSync, 0), m_Device);
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

    ResourceView<ShaderModule> D3D11Context::createShaderModule(const ShaderModuleDesc &desc) {
        auto ptr = new D3D11ShaderModule(desc, m_Device, m_DeviceContext);
        m_ShaderModules.emplace_back(ptr);
        return createResourceView(m_ShaderModules.back());
    }

    ResourceView<ShaderModule> D3D11Context::createShaderModule(const ShaderModuleFileDesc &desc) {
        auto ptr = new D3D11ShaderModule(desc, m_Device, m_DeviceContext);
        m_ShaderModules.emplace_back(ptr);
        return createResourceView(m_ShaderModules.back());
    }

    ResourceView<Shader> D3D11Context::createShader(const ShaderDesc &desc) {
        auto ptr = new D3D11Shader(desc, m_Device, m_DeviceContext);
        m_Shaders.emplace_back(ptr);
        return createResourceView(m_Shaders.back());
    }

    ResourceView<VertexBuffer> D3D11Context::createVertexBuffer(const VertexBufferDesc &desc) {
        auto ptr = new D3D11VertexBuffer(desc, m_DeviceLimits, m_Device, m_DeviceContext);
        m_VertexBuffers.emplace_back(ptr);
        return createResourceView(m_VertexBuffers.back());
    }

    ResourceView<IndexBuffer> D3D11Context::createIndexBuffer(const IndexBufferDesc &desc) {
        auto ptr = new D3D11IndexBuffer(desc, m_DeviceLimits, m_Device, m_DeviceContext);
        m_IndexBuffers.emplace_back(ptr);
        return createResourceView(m_IndexBuffers.back());
    }

    ResourceView<VertexArray> D3D11Context::createVertexArray(const VertexArrayDesc &desc) {
        VertexArray* ptr;
        if (desc.indexBuffer != nullptr) {
            ptr = new D3D11VertexArrayIndexed(desc, m_Device, m_DeviceContext);
        }
        else {
            ptr =  new D3D11VertexArray(desc, m_Device, m_DeviceContext);
        }
        m_VertexArrays.emplace_back(ptr);
        return createResourceView(m_VertexArrays.back());
    }

    ResourceView<ConstantBuffer> D3D11Context::createConstantBuffer(const ConstantBufferDesc &desc) {
        ConstantBuffer* ptr;
        auto shaderStage = desc.shaderStage;
        switch (shaderStage) {
            case SR_SHADER_VERTEX:      ptr = new D3D11ConstantBufferVertex(desc, m_DeviceLimits, m_Device, m_DeviceContext); break;
            case SR_SHADER_FRAGMENT:    ptr = new D3D11ConstantBufferPixel(desc, m_DeviceLimits, m_Device, m_DeviceContext); break;
            case SR_SHADER_GEOMETRY:    ptr = new D3D11ConstantBufferGeometry(desc, m_DeviceLimits, m_Device, m_DeviceContext); break;
            default: {
                SR_CORE_EXCEPTION("Unsupported shader stage for constant buffer");

            }
        }
        m_ConstantBuffers.emplace_back(ptr);
        return createResourceView(m_ConstantBuffers.back());
    }

    ResourceView<Texture2D> D3D11Context::createTexture2D(const Texture2DDesc& desc) {
        auto ptr = new D3D11Texture2D(desc, m_DeviceLimits, m_Device, m_DeviceContext);
        m_Textures2D.emplace_back(ptr);

        return createResourceView(m_Textures2D.back());
    }

    ResourceView<Texture2D> D3D11Context::createTexture2D(const Texture2DImageDesc &desc) {
        auto ptr = new D3D11Texture2D(desc, m_DeviceLimits, m_Device, m_DeviceContext);
        m_Textures2D.emplace_back(ptr);
        return createResourceView(m_Textures2D.back());
    }

    ResourceView<Sampler> D3D11Context::createSampler(const SamplerDesc &desc) {
        auto ptr = new D3D11Sampler(desc, m_Device, m_DeviceContext);
        m_Samplers.emplace_back(ptr);
        return createResourceView(m_Samplers.back());
    }

    ResourceView<FrameBuffer> D3D11Context::createFrameBuffer(const ResourceView<FrameBufferLayout> &desc) {
        auto ptr = new D3D11FrameBuffer(desc, m_DeviceLimits, m_Device, m_DeviceContext);
        m_FrameBuffers.emplace_back(ptr);
        return createResourceView(m_FrameBuffers.back());
    }

    ResourceView<Cubemap> D3D11Context::createCubemap(const CubemapDesc &desc) {
        auto ptr = new D3D11Cubemap(desc, m_DeviceLimits, m_Device, m_DeviceContext);
        m_Cubemaps.emplace_back(ptr);
        return createResourceView(m_Cubemaps.back());
    }

    ResourceView<Cubemap> D3D11Context::createCubemap(const CubemapImageDesc &desc) {
        auto ptr = new D3D11Cubemap(desc, m_DeviceLimits, m_Device, m_DeviceContext);
        m_Cubemaps.emplace_back(ptr);
        return createResourceView(m_Cubemaps.back());
    }

}

#endif
