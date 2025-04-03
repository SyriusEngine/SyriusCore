#include "../../../include/SyriusCore/Context/FrameBufferLayout.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    FrameBufferLayout::FrameBufferLayout(const UP<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits){

    };

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

    bool FrameBufferLayout::isValid() {
        if (m_ViewportDesc.empty()){
            SR_LOG_WARNING("FrameBufferLayout", "No viewport was added to the FrameBufferLayout");
            return false;
        }
        if (m_ColorAttachmentDesc.empty() && m_DepthStencilAttachmentDesc.empty()){
            SR_LOG_WARNING("FrameBufferLayout", "No attachments were added to the FrameBufferLayout");
            return false;
        }
        if (m_DepthStencilAttachmentDesc.size() > 1){
            SR_LOG_WARNING("FrameBufferLayout", "Multiple Depth Stencil attachments were added to FrameBufferLayout,"
                            " only the last one will be used");
        }
        if (m_ColorAttachmentDesc.size() > m_DeviceLimits->getMaxFramebufferColorAttachments()){
            SR_LOG_WARNING("FrameBufferLayout", "Number of color attachments (Color Attachments {}"
                          " exceeds the maximum number of color attachments {} supported by the device",
                          m_ColorAttachmentDesc.size(), m_DeviceLimits->getMaxFramebufferColorAttachments());
            return false;
        }
        return true;
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
