#include "../../../include/SyriusCore/Context/FrameBufferLayout.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    FrameBufferLayout::FrameBufferLayout(const Resource<DeviceLimits>& deviceLimits):
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

    void FrameBufferLayout::addCubeColorAttachmentDesc(const CubeColorAttachmentDesc &desc) {
        m_CubeColorAttachmentDesc.push_back(desc);
    }

    void FrameBufferLayout::configure() {
        if (m_ColorAttachmentDesc.empty() && m_DepthStencilAttachmentDesc.empty() && m_CubeColorAttachmentDesc.empty()){
            SR_CORE_THROW("[FrameBufferLayout]: No color or depth stencil attachment was added to the framebuffer description");
        }
        if (m_ViewportDesc.empty()){
            SR_CORE_THROW("[FrameBufferLayout]: No viewport was added to the framebuffer description");
        }
        if (m_DepthStencilAttachmentDesc.size() > 1){
            SR_CORE_WARNING("[FrameBufferLayout]: Only one depth stencil attachment is allowed, last added will be used");
        }
        if (m_CubeColorAttachmentDesc.size() > 1){
            SR_CORE_WARNING("[FrameBufferLayout]: Only one cube color attachment is allowed, last added will be used");
        }
        if (m_ColorAttachmentDesc.size() + (m_CubeColorAttachmentDesc.size() * 6) > m_DeviceLimits->getMaxFramebufferColorAttachments()){
            SR_CORE_THROW("[FrameBufferLayout]: Number of color attachments (Color Attachments %i, cube color attachments"
                          " %i) exceeds the maximum number of color attachments (%i) supported by the device",
                          m_ColorAttachmentDesc.size(), m_CubeColorAttachmentDesc.size(),
                          m_DeviceLimits->getMaxFramebufferColorAttachments());
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

    const std::vector<CubeColorAttachmentDesc> &FrameBufferLayout::getCubeColorAttachmentDesc() const {
        return m_CubeColorAttachmentDesc;
    }

}
