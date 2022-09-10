#include "../../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    FrameBuffer::FrameBuffer(const FrameBufferDesc &desc)
    : m_Width(desc.m_Width),
    m_Height(desc.m_Height),
    m_XPos(desc.m_XPos),
    m_YPos(desc.m_YPos),
    m_MinDepth(desc.m_MinDepth),
    m_MaxDepth(desc.m_MaxDepth),
    m_ClearColor{desc.m_ClearColor[0], desc.m_ClearColor[1], desc.m_ClearColor[2], desc.m_ClearColor[3]},
    m_DepthFunc{desc.m_DepthFunc},
    m_EnableDepthTest(desc.m_EnableDepthTest) {

    }

    FrameBuffer::~FrameBuffer() {

    }

    uint32 FrameBuffer::getWidth() const {
        return m_Width;
    }

    uint32 FrameBuffer::getHeight() const {
        return m_Height;
    }

    int32 FrameBuffer::getXPos() const {
        return m_XPos;
    }

    int32 FrameBuffer::getYPos() const {
        return m_YPos;
    }

    float FrameBuffer::getMinDepth() const {
        return m_MinDepth;
    }

    float FrameBuffer::getMaxDepth() const {
        return m_MaxDepth;
    }

    bool FrameBuffer::isDepthTestEnabled() const {
        return m_EnableDepthTest;
    }

    SR_DEPTH_FUNC FrameBuffer::getDepthFunc() const {
        return m_DepthFunc;
    }

    ColorAttachment *FrameBuffer::getColorAttachment(uint32 index) const {
        SR_CORE_PRECONDITION(index < m_ColorAttachments.size(), "Index out of bounds");

        return m_ColorAttachments[index];
    }

}