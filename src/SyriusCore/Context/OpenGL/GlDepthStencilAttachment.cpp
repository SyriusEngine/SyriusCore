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

    GlDepthStencilAttachment::GlDepthStencilAttachment(const DepthStencilAttachmentDesc &desc, const Resource<DeviceLimits>& deviceLimits, uint32 framebufferID):
    DepthStencilAttachment(desc, deviceLimits),
    m_GlDepthFunc(getGlComparisonFunc(desc.depthFunc)),
    m_GlStencilFunc(getGlComparisonFunc(desc.stencilFunc)),
    m_GlStencilFail(getGlStencilFunc(desc.stencilFail)),
    m_GlStencilPass(getGlStencilFunc(desc.stencilPass)),
    m_GlStencilPassDepthFail(getGlStencilFunc(desc.stencilPassDepthFail)),
    m_FrameBufferID(framebufferID),
    m_BufferID(0){

    }

    GlDepthStencilAttachment::~GlDepthStencilAttachment() = default;

    void GlDepthStencilAttachment::bind() {
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
        if (m_EnableDepthTest){
            glDisable(GL_DEPTH_TEST);
        }
        if (m_EnableStencilTest){
            glDisable(GL_STENCIL_TEST);
        }
    }

    void GlDepthStencilAttachment::clear() {
        if (m_EnableDepthTest){
            glClearNamedFramebufferfv(m_FrameBufferID, GL_DEPTH, 0, &m_ClearDepth);
        }
        if (m_EnableStencilTest){
            glClearNamedFramebufferuiv(m_FrameBufferID, GL_STENCIL, 0, &m_ClearStencil);
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

    GlDepthStencilAttachmentRenderBuffer::GlDepthStencilAttachmentRenderBuffer(const DepthStencilAttachmentDesc &desc, const Resource<DeviceLimits>& deviceLimits, uint32 framebufferID):
    GlDepthStencilAttachment(desc, deviceLimits, framebufferID) {
        m_GlInternalFormat = getGlRenderBufferFormat(desc.format);
        glCreateRenderbuffers(1, &m_BufferID);
        glNamedRenderbufferStorage(m_BufferID, m_GlInternalFormat, m_Width, m_Height);

        glNamedFramebufferRenderbuffer(m_FrameBufferID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_BufferID);
    }

    GlDepthStencilAttachmentRenderBuffer::~GlDepthStencilAttachmentRenderBuffer() {
        glDeleteRenderbuffers(1, &m_BufferID);
    }

    void GlDepthStencilAttachmentRenderBuffer::bind() {
        glBindRenderbuffer(GL_RENDERBUFFER, m_BufferID);
        GlDepthStencilAttachment::bind();
    }

    void GlDepthStencilAttachmentRenderBuffer::unbind() {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        GlDepthStencilAttachment::unbind();
    }

    void GlDepthStencilAttachmentRenderBuffer::bindShaderResource(uint32 slot) {
        SR_CORE_PRECONDITION(m_EnableShaderAccess, "[GlDepthStencilAttachment]: This attachment was created with shader "
                                                   "access disabled (enableShaderAccess = %i). The implementation picked"
                                                   "a renderbuffer object which is faster when direct access (through shaders)"
                                                   " is not needed.", m_EnableShaderAccess);
    }

    void GlDepthStencilAttachmentRenderBuffer::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        glNamedRenderbufferStorage(m_BufferID, m_GlInternalFormat, m_Width, m_Height);
    }

    GlDepthStencilAttachmentTexture::GlDepthStencilAttachmentTexture(const DepthStencilAttachmentDesc &desc, const Resource<DeviceLimits>& deviceLimits, uint32 framebufferID) :
    GlDepthStencilAttachment(desc, deviceLimits, framebufferID) {
        m_GlInternalFormat = getGlRenderBufferFormat(desc.format);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_BufferID);
        glTextureStorage2D(m_BufferID, 1, m_GlInternalFormat, m_Width, m_Height);
        glNamedFramebufferTexture(m_FrameBufferID, GL_DEPTH_STENCIL_ATTACHMENT,  m_BufferID, 0);
    }

    GlDepthStencilAttachmentTexture::~GlDepthStencilAttachmentTexture() {
        glDeleteTextures(1, &m_BufferID);
    }

    void GlDepthStencilAttachmentTexture::bind() {
        glBindTexture(GL_TEXTURE_2D, m_BufferID);
        GlDepthStencilAttachment::bind();
    }

    void GlDepthStencilAttachmentTexture::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
        GlDepthStencilAttachment::unbind();
    }

    void GlDepthStencilAttachmentTexture::bindShaderResource(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[GlDepthStencilAttachment]: Supplied slot (%i) is greater than the device number of texture slots (%i)", slot, m_DeviceLimits->getMaxTextureSlots());

        glBindTextureUnit(slot, m_BufferID);
    }

    void GlDepthStencilAttachmentTexture::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        glTextureStorage2D(m_BufferID, 1, m_GlInternalFormat, m_Width, m_Height);
    }

    GlDefaultDepthStencilAttachment::GlDefaultDepthStencilAttachment(const DepthStencilAttachmentDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    GlDepthStencilAttachment(desc, deviceLimits, 0){

    }

    GlDefaultDepthStencilAttachment::~GlDefaultDepthStencilAttachment() = default;

    void GlDefaultDepthStencilAttachment::bindShaderResource(uint32 slot) {
        SR_CORE_WARNING("[GlDefaultDepthStencilAttachment]: Attempted to bind default depth stencil attachment (%p) as shader resource at slot %i, this is not supported", this, slot)
    }

    void GlDefaultDepthStencilAttachment::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;

        // this is handled by the swapchain
    }

    Resource<Image> GlDefaultDepthStencilAttachment::getData() {
        SR_CORE_WARNING("[GlDefaultDepthStencilAttachment]: Attempted to get data from default depth stencil attachment (%p), this is not supported", this)
        return nullptr;
    }

    uint64 GlDefaultDepthStencilAttachment::getIdentifier() const {
        return 0;
    }

}
