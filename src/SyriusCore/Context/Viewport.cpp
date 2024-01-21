#include "../../../include/SyriusCore/Context/Viewport.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Viewport::Viewport(const ViewportDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_Width(desc.width),
    m_Height(desc.height),
    m_XPos(desc.xPos),
    m_YPos(desc.yPos),
    m_MinDepth(desc.minDepth),
    m_MaxDepth(desc.maxDepth){
        SR_CORE_PRECONDITION(desc.width <= m_DeviceLimits->getMaxFramebufferWidth(), "Maximum viewport width is: %i, given width is: %i", m_DeviceLimits->getMaxFramebufferWidth(), desc.width);
        SR_CORE_PRECONDITION(desc.height <= m_DeviceLimits->getMaxFramebufferHeight(), "Maximum viewport height is: %i, given height is: %i", m_DeviceLimits->getMaxFramebufferHeight(), desc.height);
        SR_CORE_PRECONDITION(desc.width != 0, "Viewport width cannot be 0");
        SR_CORE_PRECONDITION(desc.height != 0, "Viewport height cannot be 0");

    }

    Viewport::~Viewport() {

    }

    void Viewport::onResize(uint32 width, uint32 height) {
        SR_CORE_PRECONDITION(width <= m_DeviceLimits->getMaxFramebufferWidth(), "Maximum viewport width is: %i, given width is: %i", m_DeviceLimits->getMaxFramebufferWidth(), width);
        SR_CORE_PRECONDITION(height <= m_DeviceLimits->getMaxFramebufferHeight(), "Maximum viewport height is: %i, given height is: %i", m_DeviceLimits->getMaxFramebufferHeight(), height);
        SR_CORE_PRECONDITION(width != 0, "Viewport width cannot be 0");
        SR_CORE_PRECONDITION(height != 0, "Viewport height cannot be 0");

        m_Width = width;
        m_Height = height;
        bind();
    }

    void Viewport::setPos(uint32 x, uint32 y) {
        m_XPos = x;
        m_YPos = y;
        bind();
    }

    void Viewport::setDepthRange(float minDepth, float maxDepth) {
        m_MinDepth = minDepth;
        m_MaxDepth = maxDepth;
        bind();
    }

    uint32 Viewport::getWidth() const {
        return m_Width;
    }

    uint32 Viewport::getHeight() const {
        return m_Height;
    }

    uint32 Viewport::getXPos() const {
        return m_XPos;
    }

    uint32 Viewport::getYPos() const {
        return m_YPos;
    }

    float Viewport::getMinDepth() const {
        return m_MinDepth;
    }

    float Viewport::getMaxDepth() const {
        return m_MaxDepth;
    }

}
