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
      m_DepthStencilAttachment(nullptr){

    }

    FrameBuffer::~FrameBuffer() {
        for (auto attachment : m_ColorAttachments){
            delete attachment;
        }
        delete m_DepthStencilAttachment;
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

    ColorAttachment *FrameBuffer::getColorAttachment(uint32 index) const {
        SR_CORE_PRECONDITION(index < m_ColorAttachments.size(), "Index out of bounds");

        return m_ColorAttachments[index];
    }


}