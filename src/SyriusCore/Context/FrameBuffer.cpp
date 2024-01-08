#include "../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    FrameBuffer::FrameBuffer(const ResourceView<FrameBufferDescription> &desc):
    m_DepthStencilAttachment(nullptr){
        SR_CORE_PRECONDITION(desc != nullptr, "Invalid framebuffer description");
        SR_CORE_PRECONDITION(desc->getViewportDesc().size() > 0, "No viewport description was added to the framebuffer description");
        SR_CORE_PRECONDITION(!desc->getColorAttachmentDesc().empty() or
                             !desc->getDepthStencilAttachmentDesc().empty(),
                             "No color attachment description was added to the framebuffer description");

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

    void FrameBuffer::onResize(uint32 width, uint32 height) {
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

    ResourceView<Viewport> FrameBuffer::getViewport(uint32 index) {
        SR_CORE_PRECONDITION(m_Viewports.size() > 0, "No viewport was added to the framebuffer");
        SR_CORE_PRECONDITION(index < m_Viewports.size(), "Index: %i is out of bounds for viewport", index);

        return createResourceView(m_Viewports[index]);
    }

    ResourceView<ColorAttachment> FrameBuffer::getColorAttachment(uint32 index) {
        SR_CORE_PRECONDITION(m_ColorAttachments.size() > 0, "No color attachment was added to the framebuffer");
        SR_CORE_PRECONDITION(index < m_ColorAttachments.size(), "Index: %i is out of bounds for color attachment", index);

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
