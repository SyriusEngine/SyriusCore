#include "../../../../include/SyriusCore/Context/Viewport.hpp"

namespace Syrius{

    Viewport::Viewport(const ViewportDesc &desc):
    m_Width(desc.m_Width),
    m_Height(desc.m_Height),
    m_XPos(desc.m_XPos),
    m_YPos(desc.m_YPos),
    m_MinDepth(desc.m_MinDepth),
    m_MaxDepth(desc.m_MaxDepth) {

    }

    Viewport::~Viewport() {

    }

    void Viewport::setSize(uint32 width, uint32 height) {
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
