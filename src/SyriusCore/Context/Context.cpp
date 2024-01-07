#include "../../../include/SyriusCore/Context/Context.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Context::Context(const ContextDesc& desc)
    : m_VerticalSync(false),
    m_Type(desc.api),
    m_Width(desc.backBufferWidth),
    m_Height(desc.backBufferHeight){
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
        return m_FrameBuffers[0].createView();
    }

    ResourceView<VertexLayout> Context::createVertexLayout() {
        m_VertexDescriptions.emplace_back(new VertexLayout());
        return m_VertexDescriptions.back().createView();
    }

    ResourceView<FrameBufferDescription> Context::createFrameBufferDescription() {
        auto ptr = new FrameBufferDescription();
        m_FrameBufferDescriptions.emplace_back(ptr);
        return m_FrameBufferDescriptions.back().createView();
    }

    void Context::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        m_FrameBuffers[0]->onResize(width, height);
    }

    void Context::beginRenderPass(const ResourceView<FrameBuffer> &frameBuffer) {
        frameBuffer->bind();
        frameBuffer->clear();
    }

    void Context::beginRenderPass() {
        SR_CORE_PRECONDITION(m_FrameBuffers.size() > 0, "No framebuffers created");

        m_FrameBuffers[0]->bind();
        m_FrameBuffers[0]->clear();
    }

    void Context::endRenderPass(const ResourceView<FrameBuffer> &frameBuffer) {
        frameBuffer->unbind();
    }

    void Context::endRenderPass() {
        SR_CORE_PRECONDITION(m_FrameBuffers.size() > 0, "No framebuffers created");

        m_FrameBuffers[0]->unbind();
    }

    uint32 Context::getWidth() const {
        return m_Width;
    }

    uint32 Context::getHeight() const {
        return m_Height;
    }
}