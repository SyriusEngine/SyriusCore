#include "../../../include/SyriusCore/Context/FrameBufferLayout.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    FrameBufferLayout::FrameBufferLayout()= default;

    FrameBufferLayout::~FrameBufferLayout()= default;

    void FrameBufferLayout::addViewportDesc(const ViewportDesc &desc) {
        m_ViewportDesc.push_back(desc);
    }

    void FrameBufferLayout::addColorAttachmentDesc(const ColorAttachmentDesc& desc) {
        m_ColorAttachmentDesc.push_back(desc);
    }

    void FrameBufferLayout::addDepthStencilAttachmentDesc(const DepthStencilAttachmentDesc& desc) {
        m_DepthStencilAttachmentDesc.push_back(desc);
    }

    void FrameBufferLayout::configure() {
        if (m_ColorAttachmentDesc.empty() && m_DepthStencilAttachmentDesc.empty()){
            SR_CORE_EXCEPTION("No color or depth stencil attachment was added to the framebuffer description");
        }
        if (m_ViewportDesc.empty()){
            SR_CORE_EXCEPTION("No viewport was added to the framebuffer description");
        }
        if (m_DepthStencilAttachmentDesc.size() > 1){
            SR_CORE_WARNING("Only one depth stencil attachment is allowed, last added will be used");
        }

    }

    const std::vector<ViewportDesc> &FrameBufferLayout::getViewportDesc() const {
        return m_ViewportDesc;
    }

    const std::vector<ColorAttachmentDesc>& FrameBufferLayout::getColorAttachmentDesc() const {
        return m_ColorAttachmentDesc;
    }

    const std::vector<DepthStencilAttachmentDesc>& FrameBufferLayout::getDepthStencilAttachmentDesc() const {
        return m_DepthStencilAttachmentDesc;
    }

}
