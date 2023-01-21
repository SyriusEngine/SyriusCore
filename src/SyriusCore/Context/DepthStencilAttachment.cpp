#include "../../../../include/SyriusCore/Context/DepthStencilAttachment.hpp"

namespace Syrius{

    DepthStencilAttachment::DepthStencilAttachment(const DepthStencilAttachmentDesc &desc):
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format),
    m_EnableShaderRead(desc.enableShaderRead),

    m_EnableDepthTest(desc.enableDepthTest),
    m_DepthFunc(desc.depthFunc),
    m_ClearDepth(desc.clearDepth),

    m_EnableStencilTest(desc.enableStencilTest),
    m_StencilFunc(desc.stencilFunc),
    m_ClearStencil(desc.clearStencil),
    m_StencilMask(desc.stencilMask),
    m_StencilFail(desc.stencilFail),
    m_StencilPass(desc.stencilPass),
    m_StencilPassDepthFail(desc.stencilPassDepthFail) {

    }

    DepthStencilAttachment::~DepthStencilAttachment() {

    }

    uint32 DepthStencilAttachment::getWidth() const {
        return m_Width;
    }

    uint32 DepthStencilAttachment::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_DATA_FORMAT DepthStencilAttachment::getFormat() const {
        return m_Format;
    }

    bool DepthStencilAttachment::shaderReadEnabled() const {
        return m_EnableShaderRead;
    }

    bool DepthStencilAttachment::depthTestEnabled() const {
        return m_EnableDepthTest;
    }

    SR_COMPARISON_FUNC DepthStencilAttachment::getDepthFunc() const {
        return m_DepthFunc;
    }

    float DepthStencilAttachment::getClearDepth() const {
        return m_ClearDepth;
    }

    bool DepthStencilAttachment::stencilTestEnabled() const {
        return m_EnableStencilTest;
    }

    SR_COMPARISON_FUNC DepthStencilAttachment::getStencilFunc() const {
        return m_StencilFunc;
    }

    uint32 DepthStencilAttachment::getClearStencil() const {
        return m_ClearStencil;
    }

    uint32 DepthStencilAttachment::getStencilMask() const {
        return m_StencilMask;
    }

    SR_STENCIL_FUNC DepthStencilAttachment::getStencilFail() const {
        return m_StencilFail;
    }

    SR_STENCIL_FUNC DepthStencilAttachment::getStencilPass() const {
        return m_StencilPass;
    }

    SR_STENCIL_FUNC DepthStencilAttachment::getStencilPassDepthFail() const {
        return m_StencilPassDepthFail;
    }
}
