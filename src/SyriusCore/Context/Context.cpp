#include "../../../include/SyriusCore/Context/Context.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    Context::Context(const ContextDesc& desc)
    : m_VerticalSync(false),
    m_Type(desc.m_API),
    m_DefaultFrameBuffer(nullptr){

    }

    Context::~Context() {

    }

    SR_SUPPORTED_API Context::getType() const {
        return m_Type;
    }

    bool Context::isVerticalSyncEnabled() const {
        return m_VerticalSync;
    }

    VertexDescription *Context::createVertexDescription() {
        return new VertexDescription();
    }

    FrameBuffer *Context::getDefaultFrameBuffer() {
        return m_DefaultFrameBuffer;
    }

    void Context::setClearColor(float r, float g, float b, float a) {
        m_DefaultFrameBuffer->setClearColor(r, g, b, a);
    }

    void Context::setClearColor(FrameBuffer *frameBuffer, float r, float g, float b, float a) {
        frameBuffer->setClearColor(r, g, b, a);
    }

    void Context::clear() {
        m_DefaultFrameBuffer->clear();
    }

    void Context::clear(FrameBuffer *frameBuffer) {
        frameBuffer->clear();
    }

    void Context::draw(VertexArray *vao) {
        vao->drawBuffers();
    }

    void Context::drawInstanced(VertexArray *vao, uint32 instanceCount) {
        SR_CORE_PRECONDITION(instanceCount > 0, "Instance count must be greater than 0");

        vao->drawBuffersInstanced(instanceCount);
    }

    void Context::onResize(uint32 width, uint32 height) {
        SR_CORE_PRECONDITION(width > 0, "Width must be greater than 0");
        SR_CORE_PRECONDITION(height > 0, "Height must be greater than 0");
        SR_CORE_PRECONDITION(width <= getMaxFramebufferWidth(), "Width must be less than or equal to the maximum framebuffer width");
        SR_CORE_PRECONDITION(height <= getMaxFramebufferHeight(), "Height must be less than or equal to the maximum framebuffer height");

        m_DefaultFrameBuffer->setSize(width, height);
    }

    void Context::onResize(FrameBuffer *frameBuffer, uint32 width, uint32 height) {
        SR_CORE_PRECONDITION(width > 0, "Width must be greater than 0");
        SR_CORE_PRECONDITION(height > 0, "Height must be greater than 0");
        SR_CORE_PRECONDITION(width <= getMaxFramebufferWidth(), "Width must be less than or equal to the maximum framebuffer width");
        SR_CORE_PRECONDITION(height <= getMaxFramebufferHeight(), "Height must be less than or equal to the maximum framebuffer height");

        frameBuffer->setSize(width, height);
    }

}