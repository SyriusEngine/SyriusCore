#include "GlDepthStencilAttachment.hpp"

namespace Syrius{

    GLenum getGlRenderBufferFormat(SR_TEXTURE_FORMAT format){
        switch(format){
            case SR_TEXTURE_DEPTH_16: return GL_DEPTH_COMPONENT16;
            case SR_TEXTURE_DEPTH_24: return GL_DEPTH_COMPONENT24;
            case SR_TEXTURE_DEPTH_32: return GL_DEPTH_COMPONENT32;
            case SR_TEXTURE_DEPTH_24_STENCIL_8: return GL_DEPTH24_STENCIL8;
            case SR_TEXTURE_DEPTH_32_STENCIL_8: return GL_DEPTH32F_STENCIL8;
            default: {
                SR_CORE_WARNING("Invalid depth stencil format (%i), defaulting to depth 24 stencil 8", format);
                return GL_DEPTH24_STENCIL8;
            }
        }
    }

    GLenum getGlTextureBufferFormat(SR_TEXTURE_FORMAT format){
        switch(format){
            case SR_TEXTURE_DEPTH_16: return GL_DEPTH;
            case SR_TEXTURE_DEPTH_24: return GL_DEPTH;
            case SR_TEXTURE_DEPTH_32: return GL_DEPTH;
            case SR_TEXTURE_DEPTH_24_STENCIL_8: return GL_DEPTH_STENCIL;
            case SR_TEXTURE_DEPTH_32_STENCIL_8: return GL_DEPTH_STENCIL;
            default: {
                SR_CORE_WARNING("Invalid depth stencil format (%i), defaulting to depth 24 stencil 8", format);
                return GL_DEPTH_STENCIL;
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
    m_FrameBufferID(framebufferID){
        if (m_EnableShaderAccess){
            m_GlFormat = getGlTextureDataType(desc.format);

            glCreateTextures(GL_TEXTURE_2D, 1, &m_BufferID);
            glBindTexture(GL_TEXTURE_2D, m_BufferID);

            GLint supportedFormat = GL_FALSE;
            glGetInternalformativ(GL_TEXTURE_2D, m_BufferID, GL_INTERNALFORMAT_SUPPORTED, 1, &supportedFormat);
            if (supportedFormat == GL_TRUE){
                SR_CORE_OPENGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, m_GlFormat, m_Width, m_Height, 0, m_GlFormat, GL_FLOAT, nullptr));
                glTextureParameteri(m_BufferID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTextureParameteri(m_BufferID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            else{
                SR_CORE_EXCEPTION("OpenGL does not support a color attachment of usage: %i", desc.format);
            }
        }
        else{
            /*
             * Renderbuffers are supposed to be faster if you don't need to sample them in shaders
             * https://www.khronos.org/opengl/wiki/Renderbuffer_Object
             * https://www.khronos.org/opengl/wiki/Framebuffer_Object
             *
             */
            m_GlFormat = getGlRenderBufferFormat(desc.format);
            glCreateRenderbuffers(1, &m_BufferID);
            glNamedRenderbufferStorage(m_BufferID, m_GlFormat, m_Width, m_Height);
        }
    }

    GlDepthStencilAttachment::~GlDepthStencilAttachment() {
        if (m_EnableShaderAccess){
            glDeleteTextures(1, &m_BufferID);
        }
        else{
            glDeleteRenderbuffers(1, &m_BufferID);
        }
    }

    void GlDepthStencilAttachment::bind() {
        if (m_EnableShaderAccess){
            glBindTexture(GL_TEXTURE_2D, m_BufferID);
        }
        else{
            glBindRenderbuffer(GL_RENDERBUFFER, m_BufferID);
        }

        if (m_EnableDepthTest){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(m_GlDepthFunc);
            glDepthMask(m_DepthMask);
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
        SR_CORE_PRECONDITION(m_EnableShaderAccess, "Shader access is not enabled for this depth stencil attachment (enableShaderAccess = %i)", m_EnableShaderAccess);

        glBindTextureUnit(slot, m_BufferID);
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

    void GlDepthStencilAttachment::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        if (m_EnableDepthTest){
            glBindTexture(GL_TEXTURE_2D, m_BufferID);
            glTexImage2D(GL_TEXTURE_2D, 0, m_GlFormat, m_Width, m_Height, 0, m_GlFormat, GL_FLOAT, nullptr);
        }
        else{
            glNamedRenderbufferStorage(m_BufferID, m_GlFormat, m_Width, m_Height);
        }
    }

    Resource<Image> GlDepthStencilAttachment::getData() {
        return Resource<Image>();
    }

    uint64 GlDepthStencilAttachment::getIdentifier() const {
        return m_BufferID;
    }

    void GlDepthStencilAttachment::enableDepthTest(bool enable) {
        m_EnableDepthTest = enable;
    }

    void GlDepthStencilAttachment::setDepthFunc(SR_COMPARISON_FUNC func) {
        m_DepthFunc = func;
        m_GlDepthFunc = getGlComparisonFunc(func);
    }

    void GlDepthStencilAttachment::setDepthMask(SR_DEPTH_MASK mask) {
        m_DepthMask = mask;
    }

    GlDefaultDepthStencilAttachment::GlDefaultDepthStencilAttachment(const DepthStencilAttachmentDesc &desc):
    DepthStencilAttachment(desc),
    m_GlDepthFunc(getGlComparisonFunc(desc.depthFunc)),
    m_GlStencilFunc(getGlComparisonFunc(desc.stencilFunc)),
    m_GlStencilFail(getGlStencilFunc(desc.stencilFail)),
    m_GlStencilPass(getGlStencilFunc(desc.stencilPass)),
    m_GlStencilPassDepthFail(getGlStencilFunc(desc.stencilPassDepthFail)),
    m_FrameBufferID(0){

    }

    GlDefaultDepthStencilAttachment::~GlDefaultDepthStencilAttachment() {

    }

    void GlDefaultDepthStencilAttachment::bind() {
        if (m_EnableDepthTest){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(m_GlDepthFunc);
            glDepthMask(m_DepthMask);
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
        SR_CORE_WARNING("Binding default depth stencil attachment as shader resource is not supported");
    }

    void GlDefaultDepthStencilAttachment::clear() {
        if (m_EnableDepthTest){
            glClearNamedFramebufferfv(m_FrameBufferID, GL_DEPTH, 0, &m_ClearDepth);
        }
        if (m_EnableStencilTest){
            glClearNamedFramebufferuiv(m_FrameBufferID, GL_STENCIL, 0, &m_ClearStencil);
        }
    }

    void GlDefaultDepthStencilAttachment::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;

        // this is handled by the swapchain
    }

    void GlDefaultDepthStencilAttachment::enableDepthTest(bool enable) {
        m_EnableDepthTest = enable;
    }

    void GlDefaultDepthStencilAttachment::setDepthFunc(SR_COMPARISON_FUNC func) {
        m_DepthFunc = func;
        m_GlDepthFunc = getGlComparisonFunc(func);
    }

    void GlDefaultDepthStencilAttachment::setDepthMask(SR_DEPTH_MASK mask) {
        m_DepthMask = mask;
    }

    Resource<Image> GlDefaultDepthStencilAttachment::getData() {
        SR_CORE_WARNING("Read operation requested on default depth stencil attachment which is not supported");
        return {};
    }

    uint64 GlDefaultDepthStencilAttachment::getIdentifier() const {
        return 0;
    }


}
