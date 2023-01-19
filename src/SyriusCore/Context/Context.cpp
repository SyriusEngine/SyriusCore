#include "../../../include/SyriusCore/Context/Context.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    Context::Context(const ContextDesc& desc)
    : m_VerticalSync(false),
    m_Type(desc.m_API),
    m_DefaultFrameBuffer(nullptr){
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

    ResourceView<VertexDescription> Context::createVertexDescription() {
        m_VertexDescriptions.emplace_back(new VertexDescription());
        return m_VertexDescriptions.back().createView();
    }

    FrameBuffer *Context::getDefaultFrameBuffer() {
        return m_DefaultFrameBuffer;
    }


    void Context::clear(FrameBuffer *frameBuffer) {
        frameBuffer->clear();
    }

    void Context::draw(ResourceView<VertexArray> vao) {
        vao->drawBuffers();
    }

    void Context::drawInstanced(ResourceView<VertexArray> vao, uint32 instanceCount) {
        vao->drawBuffersInstanced(instanceCount);
    }

    void Context::onResize(uint32 width, uint32 height, FrameBuffer* frameBuffer) {
        SR_CORE_PRECONDITION(width > 0, "Width must be greater than 0");
        SR_CORE_PRECONDITION(height > 0, "Height must be greater than 0");
        SR_CORE_PRECONDITION(width <= getMaxFramebufferWidth(), "Width must be less than or equal to the maximum framebuffer width");
        SR_CORE_PRECONDITION(height <= getMaxFramebufferHeight(), "Height must be less than or equal to the maximum framebuffer height");

        frameBuffer->setSize(width, height);
    }

    void Context::bindDefaultFrameBuffer() {
        m_DefaultFrameBuffer->bind();
    }

    void Context::beginRenderPass(FrameBuffer *frameBuffer) {
        frameBuffer->bind();
        frameBuffer->clear();
    }

    void Context::endRenderPass(FrameBuffer *frameBuffer) {
        frameBuffer->unbind();
    }


}