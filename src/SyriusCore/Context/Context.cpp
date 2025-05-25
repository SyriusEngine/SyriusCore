#include "../../../include/SyriusCore/Context/Context.hpp"
#include "../../../include/SyriusCore/Dependencies/imgui/imgui.h"
#include "../Utils/DebugMacros.hpp"
#include "../../../include/SyriusCore/Dependencies/imgui/implot.h"

#include <algorithm>

namespace Syrius{

    Context::Context(const ContextDesc& desc):
    m_VerticalSync(false),
    m_Type(desc.api){
        SR_LOG_INFO("Context", "Creating context with backend: {}", m_Type);
    }

    Context::~Context() = default;


    SR_SUPPORTED_API Context::getType() const {
        return m_Type;
    }

    bool Context::isVerticalSyncEnabled() const {
        return m_VerticalSync;
    }

    ResourceView<FrameBuffer> Context::getDefaultFrameBuffer() {
        return createResourceView(m_FrameBuffers[0]);
    }

    ResourceView<VertexLayout> Context::createVertexLayout() {
        m_VertexDescriptions.emplace_back(new VertexLayout(m_DeviceLimits));
        return createResourceView(m_VertexDescriptions.back());
    }

    ResourceView<FrameBufferLayout> Context::createFrameBufferLayout() {
        m_FrameBufferDescriptions.emplace_back(new FrameBufferLayout(m_DeviceLimits));
        return createResourceView(m_FrameBufferDescriptions.back());
    }

    ResourceView<CubeMapLayout> Context::createCubeMapLayout(u32 width, u32 height, SR_TEXTURE_FORMAT format){
        m_CubeMapLayouts.emplace_back(new CubeMapLayout(width, height, format));
        return createResourceView(m_CubeMapLayouts.back());
    }

    void Context::onResize(const i32 width, i32 const height) const {
        SR_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        m_FrameBuffers[0]->onResize(width, height);
    }

    void Context::beginRenderPass(const ResourceView<FrameBuffer> &frameBuffer) {
        frameBuffer->bind();
    }

    void Context::beginRenderPass() {
        SR_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        m_FrameBuffers[0]->bind();
    }

    void Context::endRenderPass(const ResourceView<FrameBuffer> &frameBuffer) {
        frameBuffer->unbind();
    }

    void Context::endRenderPass() {
        SR_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        m_FrameBuffers[0]->unbind();
    }

    void Context::clear() {
        SR_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        m_FrameBuffers[0]->clear();
    }

    void Context::setClearColor(float r, float g, float b, float a) {
        SR_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        m_FrameBuffers[0]->getColorAttachment(0)->setClearColor(r, g, b, a);
    }

    float* Context::getClearColor() {
        SR_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        return m_FrameBuffers[0]->getColorAttachment(0)->getClearColor();
    }

    void Context::draw(const ResourceView<VertexArray>& vertexArray) {
        vertexArray->drawBuffers();
    }

    void Context::drawInstanced(const ResourceView<VertexArray> &vertexArray, u32 instanceCount) {
        vertexArray->drawBuffersInstanced(instanceCount);
    }

    const UP<DeviceLimits>& Context::getDeviceLimits() const {
        return m_DeviceLimits;
    }

    u32 Context::getWidth() const {
        SR_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        return m_FrameBuffers[0]->getColorAttachment(0)->getWidth();
    }

    u32 Context::getHeight() const {
        SR_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");
        return m_FrameBuffers[0]->getColorAttachment(0)->getHeight();;
    }

    void Context::enableDepthTest(bool enable) {
        SR_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");
        m_FrameBuffers[0]->enableDepthTest(enable);
    }

    template<typename T>
    inline void removeResource(std::vector<UP<T>>& resources, const ResourceView<T>& resource){
        resources.erase(std::remove_if(resources.begin(), resources.end(), [&resource](const UP<T>& res){
            return res.get() == resource.get();
        }), resources.end());
    }

    void Context::destroyShaderModule(const ResourceView<ShaderModule> &shaderModule) {
        removeResource(m_ShaderModules, shaderModule);
    }

    void Context::destroyShader(const ResourceView<Syrius::Shader> &shader) {
        removeResource(m_Shaders, shader);
    }

    void Context::destroyVertexLayout(const ResourceView<Syrius::VertexLayout> &vertexLayout) {
        removeResource(m_VertexDescriptions, vertexLayout);
    }

    void Context::destroyVertexBuffer(const ResourceView<VertexBuffer> &vertexBuffer) {
        removeResource(m_VertexBuffers, vertexBuffer);
    }

    void Context::destroyIndexBuffer(const ResourceView<IndexBuffer> &indexBuffer) {
        removeResource(m_IndexBuffers, indexBuffer);
    }

    void Context::destroyVertexArray(const ResourceView<VertexArray>& vertexArray){
        removeResource(m_VertexArrays, vertexArray);
    }

    void Context::destroyConstantBuffer(const ResourceView<ConstantBuffer>& constantBuffer){
        removeResource(m_ConstantBuffers, constantBuffer);
    }

    void Context::destroyTexture2D(const ResourceView<Syrius::Texture2D> &texture2D) {
        removeResource(m_Textures2D, texture2D);
    }

    void Context::destroySampler(const ResourceView<Sampler>& sampler){
        removeResource(m_Samplers, sampler);
    }

    void Context::destroyFrameBufferLayout(const ResourceView<FrameBufferLayout>& frameBufferLayout){
        removeResource(m_FrameBufferDescriptions, frameBufferLayout);
    }

    void Context::destroyFrameBuffer(const ResourceView<FrameBuffer>& frameBuffer){
        removeResource(m_FrameBuffers, frameBuffer);
    }

    void Context::destroyCubeMapLayout(const ResourceView<CubeMapLayout>& cubeMapLayout){
        removeResource(m_CubeMapLayouts, cubeMapLayout);
    }

    void Context::destroyCubeMap(const ResourceView<CubeMap>& cubeMap){
        removeResource(m_CubeMaps, cubeMap);
    }

    void Context::initImGui(const ImGuiDesc &desc) {
        if (m_ImGuiContextCreated) {
            SR_LOG_WARNING("Context", "ImGui already initialized!");
            return;
        }

        // Create the context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        // Enable ImPlot
        if (desc.useImPlot) {
            ImPlot::CreateContext();
            m_ImPlotContextCreated = true;
        }
        // Configure flags
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        if (desc.useDocking) {
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable docking
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        }

        // Set style
        imGuiSetStyle(desc.style);
        m_ImGuiContextCreated = true;

        SR_POSTCONDITION(m_ImGuiContextCreated == true, "Failed to create ImGui context");
    }

    void Context::terminateImGui() {
        SR_PRECONDITION(m_ImGuiContextCreated == true, "There does not exists an ImGui context");
        SR_PRECONDITION(m_IsImGuiRendering == false, "ImGuiRendering already started!")

        if (m_ImPlotContextCreated) {
            ImPlot::DestroyContext();
        }
        ImGui::DestroyContext();
        m_ImGuiContextCreated = false;
    }

    void Context::imGuiSetStyle(SR_IMGUI_STYLE style) {
        switch (style) {
            case SR_IMGUI_STYLE_DEFAULT:
            case SR_IMGUI_STYLE_DARK:       ImGui::StyleColorsDark();   break;
            case SR_IMGUI_STYLE_CLASSIC:    ImGui::StyleColorsClassic();  break;
            case SR_IMGUI_STYLE_LIGHT:      ImGui::StyleColorsLight();  break;
            default: ImGui::StyleColorsDark();
        }
    }


}