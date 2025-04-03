#include "../../../include/SyriusCore/Context/ColorAttachment.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ColorAttachment::ColorAttachment(const ColorAttachmentDesc &desc, const UP<DeviceLimits>& deviceLimits):
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format),
    m_DeviceLimits(deviceLimits),
    m_ClearColor{desc.clearColor[0], desc.clearColor[1], desc.clearColor[2], desc.clearColor[3]}{
        SR_PRECONDITION(m_Width <= m_DeviceLimits->getMaxFramebufferWidth(), "[ColorAttachment]: Supplied width {} exceeds device limit of {}", m_Width, m_DeviceLimits->getMaxFramebufferWidth());
        SR_PRECONDITION(m_Height <= m_DeviceLimits->getMaxFramebufferHeight(), "[ColorAttachment]: Supplied height {} exceeds device limit of {}", m_Height, m_DeviceLimits->getMaxFramebufferHeight());
        SR_PRECONDITION(m_Width > 0, "[ColorAttachment]: Supplied width {} must be greater than 0", m_Width);
        SR_PRECONDITION(m_Height > 0, "[ColorAttachment]: Supplied height {} must be greater than 0", m_Height);

    }

    ColorAttachment::~ColorAttachment() = default;

    void ColorAttachment::setClearColor(float r, float g, float b, float a) {
        m_ClearColor[0] = r;
        m_ClearColor[1] = g;
        m_ClearColor[2] = b;
        m_ClearColor[3] = a;
    }

    u32 ColorAttachment::getWidth() const {
        return m_Width;
    }

    u32 ColorAttachment::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_FORMAT ColorAttachment::getFormat() const {
        return m_Format;
    }

    float* ColorAttachment::getClearColor(){
        return m_ClearColor;
    }

}
