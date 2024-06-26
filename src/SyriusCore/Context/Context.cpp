#include "../../../include/SyriusCore/Context/Context.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Context::Context(const ContextDesc& desc):
    m_VerticalSync(false),
    m_Type(desc.api){
        SR_CORE_MESSAGE("Created context with backend: %s", getAPIName(m_Type).c_str());
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

    ResourceView<CubeMapLayout> Context::createCubeMapLayout(uint32 width, uint32 height, SR_TEXTURE_FORMAT format){
        m_CubeMapLayouts.emplace_back(new CubeMapLayout(width, height, format));
        return createResourceView(m_CubeMapLayouts.back());
    }

    void Context::onResize(uint32 width, uint32 height) {
        SR_CORE_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        m_FrameBuffers[0]->onResize(width, height);
    }

    void Context::beginRenderPass(const ResourceView<FrameBuffer> &frameBuffer) {
        frameBuffer->bind();
        frameBuffer->clear();
    }

    void Context::beginRenderPass() {
        SR_CORE_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        m_FrameBuffers[0]->bind();
        m_FrameBuffers[0]->clear();
    }

    void Context::endRenderPass(const ResourceView<FrameBuffer> &frameBuffer) {
        frameBuffer->unbind();
    }

    void Context::endRenderPass() {
        SR_CORE_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        m_FrameBuffers[0]->unbind();
    }

    void Context::clear() {
        SR_CORE_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        m_FrameBuffers[0]->clear();
    }

    void Context::setClearColor(float r, float g, float b, float a) {
        SR_CORE_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        m_FrameBuffers[0]->getColorAttachment(0)->setClearColor(r, g, b, a);
    }

    float* Context::getClearColor() {
        SR_CORE_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        return m_FrameBuffers[0]->getColorAttachment(0)->getClearColor();
    }

    void Context::draw(const ResourceView<VertexArray>& vertexArray) {
        vertexArray->drawBuffers();
    }

    void Context::drawInstanced(const ResourceView<VertexArray> &vertexArray, uint32 instanceCount) {
        vertexArray->drawBuffersInstanced(instanceCount);
    }

    const Resource<DeviceLimits>& Context::getDeviceLimits() const {
        return m_DeviceLimits;
    }

    uint32 Context::getWidth() const {
        SR_CORE_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");

        return m_FrameBuffers[0]->getColorAttachment(0)->getWidth();
    }

    uint32 Context::getHeight() const {
        SR_CORE_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");
        return m_FrameBuffers[0]->getColorAttachment(0)->getHeight();;
    }

    void Context::enableDepthTest(bool enable) {
        SR_CORE_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");
        m_FrameBuffers[0]->enableDepthTest(enable);
    }

    template<typename T>
    inline void removeResource(std::vector<Resource<T>>& resources, const ResourceView<T>& resource){
        resources.erase(std::remove_if(resources.begin(), resources.end(), [&resource](const Resource<T>& res){
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

}