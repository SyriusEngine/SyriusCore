#include "GlDepthStencilAttachment.hpp"

namespace Syrius{

    GlDepthStencilAttachment::GlDepthStencilAttachment(const DepthStencilAttachmentDesc &desc)
    : DepthStencilAttachment(desc),
      m_GlDepthFunc(getGlComparisonFunc(desc.m_DepthFunc)),
      m_GlStencilFunc(getGlComparisonFunc(desc.m_StencilFunc)),
      m_GlStencilFail(getGlStencilFunc(desc.m_StencilFail)),
      m_GlStencilPass(getGlStencilFunc(desc.m_StencilPass)),
      m_GlStencilPassDepthFail(getGlStencilFunc(desc.m_StencilPassDepthFail)){

        glCreateRenderbuffers(1, &m_BufferID);
        glNamedRenderbufferStorage(m_BufferID, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    }

    GlDepthStencilAttachment::~GlDepthStencilAttachment() {
        glDeleteRenderbuffers(1, &m_BufferID);
    }

    void GlDepthStencilAttachment::bind() {
        glBindRenderbuffer(GL_RENDERBUFFER, m_BufferID);

        if (m_EnableDepthTest){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(m_GlDepthFunc);
            glDepthMask(!m_DepthBufferReadOnly);
        }

        if (m_EnableStencilTest){
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(m_GlStencilFunc, m_ClearStencil, m_StencilMask);
            glStencilMask(!m_StencilBufferReadOnly);
            glStencilOp(m_GlStencilFail, m_GlStencilPassDepthFail, m_GlStencilPass);
        }
    }

    void GlDepthStencilAttachment::bindAsTexture(uint32 slot) {

    }

    void GlDepthStencilAttachment::unbind() {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        if (m_EnableDepthTest){
            glDisable(GL_DEPTH_TEST);
        }
        if (m_EnableStencilTest){
            glDisable(GL_STENCIL_TEST);
        }

    }

    void GlDepthStencilAttachment::clear() {
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void GlDepthStencilAttachment::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        glNamedRenderbufferStorage(m_BufferID, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    }

    void GlDepthStencilAttachment::setDepthFunc(SR_COMPARISON_FUNC func) {
        m_DepthFunc = func;
        m_GlDepthFunc = getGlComparisonFunc(func);
    }

    void GlDepthStencilAttachment::setDepthBufferReadOnly(bool readOnly) {
        m_DepthBufferReadOnly = readOnly;
    }

    void GlDepthStencilAttachment::setStencilFunc(SR_COMPARISON_FUNC func) {
        m_StencilFunc = func;
        m_GlStencilFunc = getGlComparisonFunc(func);
    }

    void GlDepthStencilAttachment::setStencilBufferReadOnly(bool readOnly) {
        m_StencilBufferReadOnly = readOnly;
    }

    void GlDepthStencilAttachment::setStencilMask(uint32 mask) {
        m_StencilMask = mask;
    }

    uint64 GlDepthStencilAttachment::getIdentifier() {
        return m_BufferID;
    }


    GlDefaultDepthStencilAttachment::GlDefaultDepthStencilAttachment(const DepthStencilAttachmentDesc &desc)
    : DepthStencilAttachment(desc),
    m_GlDepthFunc(getGlComparisonFunc(desc.m_DepthFunc)),
    m_GlStencilFunc(getGlComparisonFunc(desc.m_StencilFunc)),
    m_GlStencilFail(getGlStencilFunc(desc.m_StencilFail)),
    m_GlStencilPass(getGlStencilFunc(desc.m_StencilPass)),
    m_GlStencilPassDepthFail(getGlStencilFunc(desc.m_StencilPassDepthFail)){

    }

    GlDefaultDepthStencilAttachment::~GlDefaultDepthStencilAttachment() = default;

    void GlDefaultDepthStencilAttachment::bind() {
        if (m_EnableDepthTest){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(m_GlDepthFunc);
            glDepthMask(!m_DepthBufferReadOnly);
        }

        if (m_EnableStencilTest){
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(m_GlStencilFunc, m_ClearStencil, m_StencilMask);
            glStencilMask(!m_StencilBufferReadOnly);
            glStencilOp(m_GlStencilFail, m_GlStencilPassDepthFail, m_GlStencilPass);
        }

    }

    void GlDefaultDepthStencilAttachment::bindAsTexture(uint32 slot) {
        SR_CORE_WARNING("OpenGL does not allow reading from the default depth stencil attachment")
    }

    void GlDefaultDepthStencilAttachment::unbind() {
        if (m_EnableDepthTest){
            glDisable(GL_DEPTH_TEST);
        }
        if (m_EnableStencilTest){
            glDisable(GL_STENCIL_TEST);
        }
    }

    void GlDefaultDepthStencilAttachment::clear() {
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void GlDefaultDepthStencilAttachment::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
    }

    void GlDefaultDepthStencilAttachment::setDepthFunc(SR_COMPARISON_FUNC func) {
        m_DepthFunc = func;
        m_GlDepthFunc = getGlComparisonFunc(func);
    }

    void GlDefaultDepthStencilAttachment::setDepthBufferReadOnly(bool readOnly) {
        m_DepthBufferReadOnly = readOnly;
    }

    void GlDefaultDepthStencilAttachment::setStencilFunc(SR_COMPARISON_FUNC func) {
        m_StencilFunc = func;
        m_GlStencilFunc = getGlComparisonFunc(func);
    }

    void GlDefaultDepthStencilAttachment::setStencilBufferReadOnly(bool readOnly) {
        m_StencilBufferReadOnly = readOnly;
    }

    void GlDefaultDepthStencilAttachment::setStencilMask(uint32 mask) {
        m_StencilMask = mask;
    }

    uint64 GlDefaultDepthStencilAttachment::getIdentifier() {
        return 0;
    }

}
