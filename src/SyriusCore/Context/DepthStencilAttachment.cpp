#include "../../../include/SyriusCore/Context/DepthStencilAttachment.hpp"

namespace Syrius{

    DepthStencilAttachment::DepthStencilAttachment(const DepthStencilAttachmentDesc &desc)
    : m_Width(desc.m_Width), 
    m_Height(desc.m_Height),
    m_ClearDepth(desc.m_ClearDepth),
    m_ClearStencil(desc.m_ClearStencil),
    m_EnableDepthTest(desc.m_EnableDepthTest), 
    m_DepthBufferReadOnly(desc.m_DepthBufferReadOnly), 
    m_DepthFunc(desc.m_DepthFunc),
    m_EnableStencilTest(desc.m_EnableStencilTest), 
    m_StencilBufferReadOnly(desc.m_StencilBufferReadOnly), 
    m_StencilFunc(desc.m_StencilFunc), 
    m_StencilMask(desc.m_StencilMask) {
        
    }

    DepthStencilAttachment::~DepthStencilAttachment() {

    }

    uint32 DepthStencilAttachment::getWidth() const {
        return m_Width;
    }

    uint32 DepthStencilAttachment::getHeight() const {
        return m_Height;
    }

    float DepthStencilAttachment::getClearDepth() const {
        return m_ClearDepth;
    }

    uint32 DepthStencilAttachment::getClearStencil() const{
        return m_ClearStencil;
    }

    bool DepthStencilAttachment::isDepthTestEnabled() const {
        return m_EnableDepthTest;
    }

    bool DepthStencilAttachment::isDepthBufferReadOnly() const {
        return m_DepthBufferReadOnly;
    }

    SR_COMPARISON_FUNC DepthStencilAttachment::getDepthFunc() const {
        return m_DepthFunc;
    }

    bool DepthStencilAttachment::isStencilTestEnabled() const {
        return m_EnableStencilTest;
    }

    bool DepthStencilAttachment::isStencilBufferReadOnly() const {
        return m_StencilBufferReadOnly;
    }

    SR_COMPARISON_FUNC DepthStencilAttachment::getStencilFunc() const {
        return m_StencilFunc;
    }

    uint32 DepthStencilAttachment::getStencilMask() const {
        return m_StencilMask;
    }

}