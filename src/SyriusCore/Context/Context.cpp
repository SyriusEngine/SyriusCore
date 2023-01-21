#include "../../../include/SyriusCore/Context/Context.hpp"
#include "../Core/DebugMacros.hpp"

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
        return m_FrameBuffers[0].createView();
    }

    ResourceView<VertexDescription> Context::createVertexDescription() {
        m_VertexDescriptions.emplace_back(new VertexDescription());
        return m_VertexDescriptions.back().createView();
    }

    ResourceView<FrameBufferDescription> Context::createFrameBufferDescription() {
        auto ptr = new FrameBufferDescription();
        m_FrameBufferDescriptions.emplace_back(ptr);
        return m_FrameBufferDescriptions.back().createView();
    }

    void Context::onResize(uint32 width, uint32 height) {
        for (auto& frameBuffer : m_FrameBuffers){
            frameBuffer->onResize(width, height);
        }
    }


}