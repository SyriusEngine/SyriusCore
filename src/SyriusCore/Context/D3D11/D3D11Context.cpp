#include "D3D11Context.hpp"
#include "../../../../include/SyriusCore/Dependencies/imgui/implot.h"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11Context::D3D11Context(HWND &hwnd, const ContextDesc& desc):
    Context(desc),
    m_Hwnd(hwnd),
    m_DeviceContext(nullptr),
    m_Device(nullptr),
    m_SwapChain(nullptr){
        RECT clientSpace = {0, 0, 0, 0};
        GetClientRect(m_Hwnd, &clientSpace);
        const i32 width = clientSpace.right;
        const i32 height = clientSpace.bottom;

        DXGI_SWAP_CHAIN_DESC scDesc = { 0 };
        scDesc.BufferDesc.Width = width;
        scDesc.BufferDesc.Height = height;
        scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;  // DWM uses BGRA format, this is supposed to be slightly faster than RGBA
        scDesc.BufferDesc.RefreshRate.Numerator = 0;
        scDesc.BufferDesc.RefreshRate.Denominator = 0;
        scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
        scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        scDesc.SampleDesc.Count = 1;
        scDesc.SampleDesc.Quality = 0;
        scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scDesc.BufferCount = 2; // backbuffer count, is equal to 2 because of DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL
        scDesc.OutputWindow = m_Hwnd;
        scDesc.Windowed = TRUE;
        scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
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

        m_DeviceLimits = createUP<D3D11DeviceLimits>(m_Device, m_DeviceContext, m_D3DVersion);

        D3D11Context::createDefaultFrameBuffer(width, height, desc);

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

        SR_POSTCONDITION(m_Device != nullptr, "DeviceLimits is null");
        SR_POSTCONDITION(m_DeviceContext != nullptr, "DeviceContext is null");
        SR_POSTCONDITION(m_SwapChain != nullptr, "SwapChain is null");
        SR_POSTCONDITION(m_DeviceLimits.get() != nullptr, "DeviceLimits is null");
        SR_POSTCONDITION(m_FrameBuffers.size() > 0, "Default framebuffer not present");
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
        m_CubeMaps.clear();

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
        SR_PRECONDITION(m_SwapChain != nullptr, "SwapChain is nullptr");

        SR_CORE_DXGI_GET_MESSAGES();

        SR_CORE_DXGI_CHECK_DEVICE_REMOVED(m_SwapChain->Present(m_VerticalSync, 0), m_Device);
    }

    void D3D11Context::setVerticalSynchronisation(bool enable) {
        m_VerticalSync = enable;
    }

    ResourceView<ShaderModule> D3D11Context::createShaderModule(const ShaderModuleDesc &desc) {
        m_ShaderModules.emplace_back(new D3D11ShaderModule(desc, m_Device, m_DeviceContext));
        return createResourceView(m_ShaderModules.back());
    }

    ResourceView<ShaderModule> D3D11Context::createShaderModule(const ShaderModuleFileDesc &desc) {
        m_ShaderModules.emplace_back(new D3D11ShaderModule(desc, m_Device, m_DeviceContext));
        return createResourceView(m_ShaderModules.back());
    }

    ResourceView<Shader> D3D11Context::createShader(const ShaderDesc &desc) {
        m_Shaders.emplace_back(new D3D11Shader(desc, m_Device, m_DeviceContext));
        return createResourceView(m_Shaders.back());
    }

    ResourceView<VertexBuffer> D3D11Context::createVertexBuffer(const VertexBufferDesc &desc) {
        m_VertexBuffers.emplace_back(new D3D11VertexBuffer(desc, m_DeviceLimits, m_Device, m_DeviceContext));
        return createResourceView(m_VertexBuffers.back());
    }

    ResourceView<IndexBuffer> D3D11Context::createIndexBuffer(const IndexBufferDesc &desc) {
        m_IndexBuffers.emplace_back(new D3D11IndexBuffer(desc, m_DeviceLimits, m_Device, m_DeviceContext));
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
                SR_LOG_THROW("D3D11Context", "Unsupported shader stage () for constant buffer", shaderStage);
            }
        }
        m_ConstantBuffers.emplace_back(ptr);
        return createResourceView(m_ConstantBuffers.back());
    }

    ResourceView<Texture2D> D3D11Context::createTexture2D(const Texture2DDesc& desc) {
        m_Textures2D.emplace_back(new D3D11Texture2D(desc, m_DeviceLimits, m_Device, m_DeviceContext));
        return createResourceView(m_Textures2D.back());
    }

    ResourceView<Texture2D> D3D11Context::createTexture2D(const Texture2DImageDesc &desc) {
        m_Textures2D.emplace_back(new D3D11Texture2D(desc, m_DeviceLimits, m_Device, m_DeviceContext));
        return createResourceView(m_Textures2D.back());
    }

    ResourceView<Sampler> D3D11Context::createSampler(const SamplerDesc &desc) {
        m_Samplers.emplace_back(new D3D11Sampler(desc, m_Device, m_DeviceContext));
        return createResourceView(m_Samplers.back());
    }

    ResourceView<FrameBuffer> D3D11Context::createFrameBuffer(const ResourceView<FrameBufferLayout> &desc) {
        m_FrameBuffers.emplace_back(new D3D11FrameBuffer(desc, m_DeviceLimits, m_Device, m_DeviceContext));
        return createResourceView(m_FrameBuffers.back());
    }

    ResourceView<CubeMap> D3D11Context::createCubeMap(const ResourceView<Syrius::CubeMapLayout> &desc) {
        m_CubeMaps.emplace_back(new D3D11CubeMap(desc, m_DeviceLimits, m_Device, m_DeviceContext));
        return createResourceView(m_CubeMaps.back());
    }

    void D3D11Context::initImGui(const ImGuiDesc& desc) {
        Context::initImGui(desc);

        ImGui_ImplWin32_Init(m_Hwnd);
        ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
    }

    void D3D11Context::terminateImGui() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();

        Context::terminateImGui();
    }

    void D3D11Context::onImGuiBegin() {
        SR_PRECONDITION(m_ImGuiContextCreated == true, "There does not exists an ImGui context");
        SR_PRECONDITION(m_IsImGuiRendering == false, "ImGuiRendering already started!")

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        m_IsImGuiRendering = true;

        SR_POSTCONDITION(m_IsImGuiRendering == true, "Failed to start ImGuiRendering");
    }

    void D3D11Context::onImGuiEnd() {
        SR_PRECONDITION(m_IsImGuiRendering == true, "onImGuiEnd() called before onImGuiBegin()")

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
        m_IsImGuiRendering = false;
    }

    void D3D11Context::createDefaultFrameBuffer(const i32 width, const  i32 height, const ContextDesc &desc) {
        auto defaultFbDesc = createFrameBufferLayout();

        ViewportDesc viewportDesc;
        viewportDesc.width = width;
        viewportDesc.height = height;
        defaultFbDesc->addViewportDesc(viewportDesc);

        ColorAttachmentDesc colorAttachmentDesc;
        colorAttachmentDesc.width = width;
        colorAttachmentDesc.height = height;
        defaultFbDesc->addColorAttachmentDesc(colorAttachmentDesc);

        if (desc.enableDepthTest or desc.enableStencilTest){
            DepthStencilAttachmentDesc dsaDesc;
            dsaDesc.width = width;
            dsaDesc.height = height;
            dsaDesc.enableDepthTest = desc.enableDepthTest;
            dsaDesc.enableStencilTest = desc.enableStencilTest;
            defaultFbDesc->addDepthStencilAttachmentDesc(dsaDesc);
        }

        auto ptr = new D3D11DefaultFrameBuffer(defaultFbDesc, m_DeviceLimits, m_Device, m_DeviceContext, m_SwapChain);
        m_FrameBuffers.emplace_back(ptr);
    }


}

#endif
