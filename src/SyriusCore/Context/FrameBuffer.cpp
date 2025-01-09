#include "../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    FrameBuffer::FrameBuffer(const ResourceView<FrameBufferLayout> &desc, const UP<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_DepthStencilAttachment(nullptr){
        SR_LOG_THROW_IF_FALSE(desc->isValid(), "FrameBuffer", "Framebuffer layout (%p) is not valid", desc.get());
    }

    FrameBuffer::~FrameBuffer() {
        for (auto& viewport : m_Viewports){
            viewport.reset();
        }
        for (auto& colorAttachment : m_ColorAttachments){
            colorAttachment.reset();
        }
        m_DepthStencilAttachment.reset();
    }

    void FrameBuffer::clear() {
        for (const auto &attachment: m_ColorAttachments){
            attachment->clear();
        }
        m_DepthStencilAttachment->clear();
    }

    void FrameBuffer::onResize(u32 width, u32 height) {
        for (auto& viewport : m_Viewports){
            viewport->onResize(width, height);
        }
        for (auto& colorAttachment : m_ColorAttachments){
            colorAttachment->onResize(width, height);
        }
        m_DepthStencilAttachment->onResize(width, height);
    }

    void FrameBuffer::enableDepthTest(bool enable) {
        m_DepthStencilAttachment->enableDepthTest(enable);
    }

    ResourceView<Viewport> FrameBuffer::getViewport(u32 index) {
        SR_PRECONDITION(m_Viewports.size() > 0, "No viewport was added to the framebuffer");
        SR_PRECONDITION(index < m_Viewports.size(), "Index: %i is out of bounds for viewport", index);

        return createResourceView(m_Viewports[index]);
    }

    ResourceView<ColorAttachment> FrameBuffer::getColorAttachment(u32 index) {
        SR_PRECONDITION(m_ColorAttachments.size() > 0, "No color attachment was added to the framebuffer");
        SR_PRECONDITION(index < m_ColorAttachments.size(), "Index: %i is out of bounds for color attachment", index);

        return createResourceView(m_ColorAttachments[index]);
    }

    ResourceView<DepthStencilAttachment> FrameBuffer::getDepthStencilAttachment() {
        return createResourceView(m_DepthStencilAttachment);
    }

    void FrameBuffer::setDepthFunc(SR_COMPARISON_FUNC func) {
        m_DepthStencilAttachment->setDepthFunc(func);
    }

    void FrameBuffer::setDepthMask(SR_DEPTH_MASK mask) {
        m_DepthStencilAttachment->setDepthMask(mask);
    }

}
