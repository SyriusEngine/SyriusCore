#include "../../../include/SyriusCore/Context/CubeColorAttachment.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    CubeColorAttachment::CubeColorAttachment(const CubeColorAttachmentDesc &desc, const Resource<DeviceLimits> &deviceLimits):
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format),
    m_DeviceLimits(deviceLimits),
    m_ClearColor{desc.clearColor[0], desc.clearColor[1], desc.clearColor[2], desc.clearColor[3]}{
        SR_CORE_PRECONDITION(m_Width <= m_DeviceLimits->getMaxFramebufferWidth(), "[CubeColorAttachment]: Supplied width (%i) exceeds device limit of %i", m_Width, m_DeviceLimits->getMaxFramebufferWidth());
        SR_CORE_PRECONDITION(m_Height <= m_DeviceLimits->getMaxFramebufferHeight(), "[CubeColorAttachment]: Supplied height (%i) exceeds device limit of %i", m_Height, m_DeviceLimits->getMaxFramebufferHeight());
        SR_CORE_PRECONDITION(m_Width > 0, "[CubeColorAttachment]: Supplied width (%i) must be greater than 0", m_Width);
        SR_CORE_PRECONDITION(m_Height > 0, "[CubeColorAttachment]: Supplied height (%i) must be greater than 0", m_Height);

    }

    CubeColorAttachment::~CubeColorAttachment() = default;

    void CubeColorAttachment::setClearColor(float r, float g, float b, float a) {
        m_ClearColor[0] = r;
        m_ClearColor[1] = g;
        m_ClearColor[2] = b;
        m_ClearColor[3] = a;
    }

    uint32 CubeColorAttachment::getWidth() const {
        return m_Width;
    }

    uint32 CubeColorAttachment::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_FORMAT CubeColorAttachment::getFormat() const {
        return m_Format;
    }

    std::vector<float> &CubeColorAttachment::getClearColor() {
        return m_ClearColor;
    }
}
