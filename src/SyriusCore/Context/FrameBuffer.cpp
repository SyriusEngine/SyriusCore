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
    m_EnableDepthTest(desc.m_EnableDepthTest),
    m_DepthBufferReadOnly(desc.m_DepthBufferReadOnly),
    m_EnableStencilTest(desc.m_EnableStencilTest),
    m_ClearDepth(desc.m_ClearDepth),
    m_StencilBufferReadOnly(desc.m_StencilBufferReadOnly),
    m_ClearColor{desc.m_ClearColor[0], desc.m_ClearColor[1], desc.m_ClearColor[2], desc.m_ClearColor[3]},
    m_DepthFunc{desc.m_DepthFunc},
    m_StencilFunc{desc.m_StencilFunc},
    m_StencilReference{desc.m_StencilReference},
    m_StencilMask{desc.m_StencilMask}{

    }

    FrameBuffer::~FrameBuffer() {
        for (auto attachment : m_ColorAttachments){
            delete attachment;
        }
    }

    void  FrameBuffer::setDepthBufferReadOnly(bool readOnly){
        m_DepthBufferReadOnly = readOnly;
    }

    void  FrameBuffer::setStencilBufferReadOnly(bool readOnly){
        m_StencilBufferReadOnly = readOnly;
    }

    void FrameBuffer::setClearColor(float red, float green, float blue, float alpha) {
        m_ClearColor[0] = red;
        m_ClearColor[1] = green;
        m_ClearColor[2] = blue;
        m_ClearColor[3] = alpha;
    }

    void FrameBuffer::setClearDepth(float depth) {
        m_ClearDepth = depth;
    }

    void FrameBuffer::setStencilReference(uint32 reference) {
        m_StencilReference = reference;
    }

    void FrameBuffer::setStencilMask(uint32 mask) {
        m_StencilMask = mask;
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

    SR_COMPARISON_FUNC FrameBuffer::getDepthFunc() const {
        return m_DepthFunc;
    }

    bool FrameBuffer::isDepthBufferReadOnly() const {
        return m_DepthBufferReadOnly;
    }

    bool FrameBuffer::isStencilBufferReadOnly() const {
        return m_StencilBufferReadOnly;
    }

    SR_COMPARISON_FUNC FrameBuffer::getStencilFunc() const {
        return m_StencilFunc;
    }

    uint32 FrameBuffer::getStencilReference() const {
        return m_StencilReference;
    }

    uint32 FrameBuffer::getStencilMask() const {
        return m_StencilMask;
    }

    ColorAttachment *FrameBuffer::getColorAttachment(uint32 index) const {
        SR_CORE_PRECONDITION(index < m_ColorAttachments.size(), "Index out of bounds");

        return m_ColorAttachments[index];
    }


}