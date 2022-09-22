#include "../../../include/SyriusCore/Context/Context.hpp"

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

}