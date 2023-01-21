#include "../../../../include/SyriusCore/Context/Viewport.hpp"

namespace Syrius{

    Viewport::Viewport(const ViewportDesc &desc):
    m_Width(desc.width),
    m_Height(desc.height),
    m_XPos(desc.xPos),
    m_YPos(desc.yPos),
    m_MinDepth(desc.minDepth),
    m_MaxDepth(desc.maxDepth),
    m_EnableAutoResize(desc.enableAutoResize){

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

    void Viewport::onResize(uint32 width, uint32 height) {
        if(m_EnableAutoResize){
            setSize(width, height);
        }
    }

}
