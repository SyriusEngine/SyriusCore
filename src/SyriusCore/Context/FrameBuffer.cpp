#include "../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    FrameBuffer::FrameBuffer(const ResourceView<FrameBufferLayout> &desc, const Resource<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_DepthStencilAttachment(nullptr){
        if (!desc->isValid()){
            SR_CORE_THROW("[FrameBuffer]: Framebuffer layout (%p) is not valid", desc.get());
        }
    }

    FrameBuffer::~FrameBuffer() {
        for (auto& viewport : m_Viewports){
            viewport.reset();
        }
        for (auto& colorAttachment : m_ColorAttachments){
            colorAttachment.reset();
        }
        for (auto& cubeColorAttachment : m_CubeColorAttachments){
            cubeColorAttachment.reset();
        }
        m_DepthStencilAttachment.reset();
    }

    void FrameBuffer::clear() {
        for (const auto &attachment: m_ColorAttachments){
            attachment->clear();
        }
        for (const auto &attachment: m_CubeColorAttachments){
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
        for (auto& cubeColorAttachment : m_CubeColorAttachments){
            cubeColorAttachment->onResize(width, height);
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

    ResourceView<CubeColorAttachment> FrameBuffer::getCubeColorAttachment(uint32 index) {
        SR_CORE_PRECONDITION(m_CubeColorAttachments.size() > 0, "No cube color attachment was added to the framebuffer");
        SR_CORE_PRECONDITION(index < m_CubeColorAttachments.size(), "Index: %i is out of bounds for cube color attachment", index);

        return createResourceView(m_CubeColorAttachments[index]);
    }

    void FrameBuffer::setDepthFunc(SR_COMPARISON_FUNC func) {
        m_DepthStencilAttachment->setDepthFunc(func);
    }

    void FrameBuffer::setDepthMask(SR_DEPTH_MASK mask) {
        m_DepthStencilAttachment->setDepthMask(mask);
    }

}
