#include "GlFrameBuffer.hpp"

namespace Syrius{

    GlFrameBuffer::GlFrameBuffer(const FrameBufferDesc &desc)
    : FrameBuffer(desc),
    m_FrameBufferID(0),
    m_ClearMask(GL_COLOR_BUFFER_BIT),
    m_GlDepthFunc(getGlDepthFunc(desc.m_DepthFunc)),
    m_RenderBuffer(nullptr){
        SR_CORE_PRECONDITION(!desc.m_ColorAttachments.empty(), "Framebuffer must have at least one color attachment!");

        glCreateFramebuffers(1, &m_FrameBufferID);

        GLenum attachmentIndex = GL_COLOR_ATTACHMENT0;
        for (const auto& format : desc.m_ColorAttachments){
            auto colorAttachment = new GlColorAttachment(m_Width, m_Height, format);
            m_ColorAttachments.push_back(colorAttachment);
            glNamedFramebufferTexture(m_FrameBufferID, attachmentIndex, colorAttachment->getIdentifier(), 0);
            attachmentIndex++;
        }

        if (desc.m_EnableDepthTest){
            m_ClearMask |= GL_DEPTH_BUFFER_BIT;
            m_RenderBuffer = new GlRenderBuffer(m_Width, m_Height);
            glNamedFramebufferRenderbuffer(m_FrameBufferID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer->getIdentifier());
        }

        SR_CORE_POSTCONDITION(glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer creation failed!");
    }

    GlFrameBuffer::~GlFrameBuffer() {
        for (auto attachment : m_ColorAttachments){
            delete attachment;
        }
        delete m_RenderBuffer;

        glDeleteFramebuffers(1, &m_FrameBufferID);
    }

    void GlFrameBuffer::bind() {
        SR_CORE_PRECONDITION(glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Cannot bind the framebuffer because the framebuffer is not complete");

        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
        glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);

        // call every time a different framebuffer is bound, this must happen because the viewport is not a part of the framebuffer state
        glViewport(m_XPos, m_YPos, m_Width, m_Height);

        if (m_EnableDepthTest){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(m_GlDepthFunc);
            m_RenderBuffer->bind();
        }
        else{
            glDisable(GL_DEPTH_TEST);
        }
    }

    void GlFrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GlFrameBuffer::setClearColor(float red, float green, float blue, float alpha) {
        m_ClearColor[0] = red;
        m_ClearColor[1] = green;
        m_ClearColor[2] = blue;
        m_ClearColor[3] = alpha;
    }

    void GlFrameBuffer::setPosition(int32 xPos, int32 yPos) {
        m_XPos = xPos;
        m_YPos = yPos;
    }

    void GlFrameBuffer::setDepthFunc(SR_DEPTH_FUNC func) {
        m_DepthFunc = func;
        m_GlDepthFunc = getGlDepthFunc(func);
    }

    void GlFrameBuffer::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        for (auto& colorAttachment : m_ColorAttachments){
            colorAttachment->onResize(width, height);
        }
    }

    void GlFrameBuffer::enableDepthTest() {
        m_EnableDepthTest = true;
    }

    void GlFrameBuffer::disableDepthTest() {
        m_EnableDepthTest = false;
    }

    void GlFrameBuffer::clear() {
        bind();
        glClear(m_ClearMask);
    }

    void GlFrameBuffer::bindColorAttachment(uint32 slot, uint32 attachmentIndex) {
        SR_CORE_PRECONDITION(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of bounds!");

        m_ColorAttachments[attachmentIndex]->bind(slot);
    }

    Image *GlFrameBuffer::readColorAttachment(uint32 attachmentIndex) {
        SR_CORE_PRECONDITION(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of bounds!");

        return m_ColorAttachments[attachmentIndex]->readColorAttachment();
    }

    GlDefaultFramebuffer::GlDefaultFramebuffer(const FrameBufferDesc &desc)
    : FrameBuffer(desc),
    m_GlDepthFunc(getGlDepthFunc(desc.m_DepthFunc)),
    m_ClearMask(GL_COLOR_BUFFER_BIT){
        if (desc.m_EnableDepthTest){
            m_ClearMask |= GL_DEPTH_BUFFER_BIT;
        }
    }

    GlDefaultFramebuffer::~GlDefaultFramebuffer() {

    }

    void GlDefaultFramebuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // call every time a different framebuffer is bound, this must happen because the viewport is not a part of the framebuffer state
        glViewport(m_XPos, m_YPos, m_Width, m_Height);
        glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);

        if (m_EnableDepthTest){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(m_GlDepthFunc);
        }
        else{
            glDisable(GL_DEPTH_TEST);
        }
    }

    void GlDefaultFramebuffer::unbind() {

    }

    void GlDefaultFramebuffer::setClearColor(float red, float green, float blue, float alpha) {
        m_ClearColor[0] = red;
        m_ClearColor[1] = green;
        m_ClearColor[2] = blue;
        m_ClearColor[3] = alpha;
    }

    void GlDefaultFramebuffer::setPosition(int32 xPos, int32 yPos) {
        m_XPos = xPos;
        m_YPos = yPos;
    }

    void GlDefaultFramebuffer::setDepthFunc(SR_DEPTH_FUNC func) {
        m_DepthFunc = func;
        m_GlDepthFunc = getGlDepthFunc(func);
    }

    void GlDefaultFramebuffer::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
    }

    void GlDefaultFramebuffer::enableDepthTest() {
        m_EnableDepthTest = true;
    }

    void GlDefaultFramebuffer::disableDepthTest() {
        m_EnableDepthTest = false;
    }

    void GlDefaultFramebuffer::clear() {
        bind();
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GlDefaultFramebuffer::bindColorAttachment(uint32 slot, uint32 attachmentIndex) {
        SR_CORE_WARNING("Default framebuffer does not support color attachments!");
    }

    Image *GlDefaultFramebuffer::readColorAttachment(uint32 attachmentIndex) {
        SR_CORE_WARNING("Reading the color attachment of the default framebuffer is not supported!");

        return nullptr;
    }
}
