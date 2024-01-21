#include "../../../include/SyriusCore/Context/Context.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Context::Context(const ContextDesc& desc)
    : m_VerticalSync(false),
    m_Type(desc.api){
        SR_CORE_MESSAGE("Created context with backend: " + getAPIName(m_Type));
    }

    Context::~Context() {

    }

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

    ResourceView<FrameBufferDescription> Context::createFrameBufferDescription() {
        auto ptr = new FrameBufferDescription();
        m_FrameBufferDescriptions.emplace_back(ptr);
        return createResourceView(m_FrameBufferDescriptions.back());
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
        return m_FrameBuffers[0]->getColorAttachment(0)->getHeight();;
    }
}