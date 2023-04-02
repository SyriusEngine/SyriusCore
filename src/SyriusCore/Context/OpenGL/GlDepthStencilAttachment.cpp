#include "GlDepthStencilAttachment.hpp"

namespace Syrius{

    GLenum getGlDepthStencilFormat(SR_TEXTURE_FORMAT format){
        switch(format){
            case SR_TEXTURE_DEPTH_16: return GL_DEPTH_COMPONENT16;
            case SR_TEXTURE_DEPTH_24: return GL_DEPTH_COMPONENT24;
            case SR_TEXTURE_DEPTH_32: return GL_DEPTH_COMPONENT32;
            case SR_TEXTURE_DEPTH_24_STENCIL_8: return GL_DEPTH24_STENCIL8;
            case SR_TEXTURE_DEPTH_32_STENCIL_8: return GL_DEPTH32F_STENCIL8;
            default: {
                SR_CORE_WARNING("Invalid depth stencil format, defaulting to depth 24 stencil 8");
                return GL_DEPTH24_STENCIL8;
            }
        }
    }

    GlDepthStencilAttachment::GlDepthStencilAttachment(const DepthStencilAttachmentDesc &desc, uint32 framebufferID):
    DepthStencilAttachment(desc),
    m_GlDepthFunc(getGlComparisonFunc(desc.depthFunc)),
    m_GlStencilFunc(getGlComparisonFunc(desc.stencilFunc)),
    m_GlStencilFail(getGlStencilFunc(desc.stencilFail)),
    m_GlStencilPass(getGlStencilFunc(desc.stencilPass)),
    m_GlStencilPassDepthFail(getGlStencilFunc(desc.stencilPassDepthFail)),
    m_GlFormat(getGlDepthStencilFormat(desc.format)),
    m_FrameBufferID(framebufferID){
        glCreateRenderbuffers(1, &m_BufferID);
        glNamedRenderbufferStorage(m_BufferID, m_GlFormat, m_Width, m_Height);
    }

    GlDepthStencilAttachment::~GlDepthStencilAttachment() {
        glDeleteRenderbuffers(1, &m_BufferID);

    }

    void GlDepthStencilAttachment::bind() {
        glBindRenderbuffer(GL_RENDERBUFFER, m_BufferID);

        if (m_EnableDepthTest){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(m_GlDepthFunc);
        }
        else{
            glDisable(GL_DEPTH_TEST);
        }

        if (m_EnableStencilTest){
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(m_GlStencilFunc, m_ClearStencil, m_StencilMask);
            glStencilOp(m_GlStencilFail, m_GlStencilPassDepthFail, m_GlStencilPass);
        }
        else{
            glDisable(GL_STENCIL_TEST);
        }
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

    void GlDepthStencilAttachment::bindShaderResource(uint32 slot) {
        SR_CORE_WARNING("Read operation requested on depth stencil attachment with creation flag: enableShaderRead = false");

    }

    void GlDepthStencilAttachment::clear() {
//        glClearNamedFramebufferfi(m_FrameBufferID, GL_DEPTH_STENCIL, 0, m_ClearDepth, m_ClearStencil);

        if (m_EnableDepthTest){
            glClearNamedFramebufferfv(m_FrameBufferID, GL_DEPTH, 0, &m_ClearDepth);
        }
        if (m_EnableStencilTest){
            glClearNamedFramebufferuiv(m_FrameBufferID, GL_STENCIL, 0, &m_ClearStencil);
        }
    }

    void GlDepthStencilAttachment::setSize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        glNamedRenderbufferStorage(m_BufferID, m_GlFormat, m_Width, m_Height);

    }

    Resource<Image> GlDepthStencilAttachment::getData() {
        return Resource<Image>();
    }

    uint64 GlDepthStencilAttachment::getIdentifier() const {
        return m_BufferID;
    }

    GlDefaultDepthStencilAttachment::GlDefaultDepthStencilAttachment(const DepthStencilAttachmentDesc &desc, uint32 framebufferID):
    DepthStencilAttachment(desc),
    m_GlDepthFunc(getGlComparisonFunc(desc.depthFunc)),
    m_GlStencilFunc(getGlComparisonFunc(desc.stencilFunc)),
    m_GlStencilFail(getGlStencilFunc(desc.stencilFail)),
    m_GlStencilPass(getGlStencilFunc(desc.stencilPass)),
    m_GlStencilPassDepthFail(getGlStencilFunc(desc.stencilPassDepthFail)),
    m_FrameBufferID(framebufferID){

    }

    GlDefaultDepthStencilAttachment::~GlDefaultDepthStencilAttachment() {

    }

    void GlDefaultDepthStencilAttachment::bind() {
        if (m_EnableDepthTest){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(m_GlDepthFunc);
        }
        else{
            glDisable(GL_DEPTH_TEST);
        }

        if (m_EnableStencilTest){
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(m_GlStencilFunc, m_ClearStencil, m_StencilMask);
            glStencilOp(m_GlStencilFail, m_GlStencilPassDepthFail, m_GlStencilPass);
        }
        else{
            glDisable(GL_STENCIL_TEST);
        }
    }

    void GlDefaultDepthStencilAttachment::unbind() {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
    }

    void GlDefaultDepthStencilAttachment::bindShaderResource(uint32 slot) {

    }

    void GlDefaultDepthStencilAttachment::clear() {
        if (m_EnableDepthTest){
            glClearNamedFramebufferfv(m_FrameBufferID, GL_DEPTH, 0, &m_ClearDepth);
        }
        if (m_EnableStencilTest){
            glClearNamedFramebufferuiv(m_FrameBufferID, GL_STENCIL, 0, &m_ClearStencil);
        }
    }

    void GlDefaultDepthStencilAttachment::setSize(uint32 width, uint32 height) {

    }

    Resource<Image> GlDefaultDepthStencilAttachment::getData() {
        return Resource<Image>();
    }

    uint64 GlDefaultDepthStencilAttachment::getIdentifier() const {
        return 0;
    }

}
