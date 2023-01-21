#include "GlViewport.hpp"

namespace Syrius{

    GlViewport::GlViewport(const ViewportDesc &desc):
    Viewport(desc) {

    }

    GlViewport::~GlViewport() {

    }

    void GlViewport::bind() {
        glViewport(m_XPos, m_YPos, m_Width, m_Height);
        glDepthRange(m_MinDepth, m_MaxDepth);
    }
}
