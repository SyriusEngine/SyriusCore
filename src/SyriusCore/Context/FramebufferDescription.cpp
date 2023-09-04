#include "../../../include/SyriusCore/Context/FramebufferDescription.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    FrameBufferDescription::FrameBufferDescription(){

    }

    void FrameBufferDescription::addViewportDesc(const ViewportDesc &desc) {
        m_ViewportDesc.push_back(desc);
    }

    void FrameBufferDescription::addColorAttachmentDesc(const ColorAttachmentDesc& desc) {
        m_ColorAttachmentDesc.push_back(desc);
    }

    void FrameBufferDescription::addDepthStencilAttachmentDesc(const DepthStencilAttachmentDesc& desc) {
        m_DepthStencilAttachmentDesc.push_back(desc);
    }

    void FrameBufferDescription::configure() {
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

    const std::vector<ViewportDesc> &FrameBufferDescription::getViewportDesc() const {
        return m_ViewportDesc;
    }

    const std::vector<ColorAttachmentDesc>& FrameBufferDescription::getColorAttachmentDesc() const {
        return m_ColorAttachmentDesc;
    }

    const std::vector<DepthStencilAttachmentDesc>& FrameBufferDescription::getDepthStencilAttachmentDesc() const {
        return m_DepthStencilAttachmentDesc;
    }

}
