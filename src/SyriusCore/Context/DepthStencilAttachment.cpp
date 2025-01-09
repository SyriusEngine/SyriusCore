#include "../../../include/SyriusCore/Context/DepthStencilAttachment.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    DepthStencilAttachment::DepthStencilAttachment(const DepthStencilAttachmentDesc &desc, const UP<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format),
    m_EnableShaderAccess(desc.enableShaderAccess),

    m_EnableDepthTest(desc.enableDepthTest),
    m_DepthFunc(desc.depthFunc),
    m_ClearDepth(desc.clearDepth),
    m_DepthMask(desc.depthMask),

    m_EnableStencilTest(desc.enableStencilTest),
    m_StencilFunc(desc.stencilFunc),
    m_ClearStencil(desc.clearStencil),
    m_StencilMask(desc.stencilMask),
    m_StencilFail(desc.stencilFail),
    m_StencilPass(desc.stencilPass),
    m_StencilPassDepthFail(desc.stencilPassDepthFail){

    }

    DepthStencilAttachment::~DepthStencilAttachment() {

    }

    u32 DepthStencilAttachment::getWidth() const {
        return m_Width;
    }

    u32 DepthStencilAttachment::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_FORMAT DepthStencilAttachment::getFormat() const {
        return m_Format;
    }

    bool DepthStencilAttachment::shaderReadEnabled() const {
        return m_EnableShaderAccess;
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

    u32 DepthStencilAttachment::getClearStencil() const {
        return m_ClearStencil;
    }

    u32 DepthStencilAttachment::getStencilMask() const {
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

    void DepthStencilAttachment::setClearDepth(float depth) {
        m_ClearDepth = depth;
    }
}
