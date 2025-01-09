#include "GlViewport.hpp"

namespace Syrius{

    GlViewport::GlViewport(const ViewportDesc &desc, const UP<DeviceLimits>& deviceLimits):
    Viewport(desc, deviceLimits) {

    }

    GlViewport::~GlViewport() {

    }

    void GlViewport::bind() {
        glViewport(m_XPos, m_YPos, m_Width, m_Height);
        glDepthRange(m_MinDepth, m_MaxDepth);
    }
}
