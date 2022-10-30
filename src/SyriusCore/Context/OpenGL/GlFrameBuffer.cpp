#include "GlFrameBuffer.hpp"

namespace Syrius{

    GlFrameBuffer::GlFrameBuffer(const FrameBufferDesc &desc)
    : FrameBuffer(desc),
    m_FrameBufferID(0),
    m_EnableDepthTest(desc.m_EnableDepthTest),
    m_EnableStencilTest(desc.m_EnableStencilTest){
        SR_CORE_PRECONDITION(!desc.m_ColorAttachments.empty(), "Framebuffer must have at least one color attachment!");

        glCreateFramebuffers(1, &m_FrameBufferID);

        GLenum attachmentIndex = GL_COLOR_ATTACHMENT0;
        for (const auto& colorAttachmentDesc : desc.m_ColorAttachments){
            auto colorAttachment = new GlColorAttachment(colorAttachmentDesc);
            m_ColorAttachments.push_back(colorAttachment);
            glNamedFramebufferTexture(m_FrameBufferID, attachmentIndex, colorAttachment->getIdentifier(), 0);
            attachmentIndex++;
        }

        if (desc.m_EnableDepthTest or desc.m_EnableStencilTest){
            DepthStencilAttachmentDesc dsaDesc;
            dsaDesc.m_Width = m_Width;
            dsaDesc.m_Height = m_Height;
            dsaDesc.m_EnableDepthTest = desc.m_EnableDepthTest;
            dsaDesc.m_DepthBufferReadOnly = desc.m_DepthBufferReadOnly;
            dsaDesc.m_DepthFunc = desc.m_DepthFunc;
            dsaDesc.m_EnableStencilTest = desc.m_EnableStencilTest;
            dsaDesc.m_StencilBufferReadOnly = desc.m_StencilBufferReadOnly;
            dsaDesc.m_StencilFunc = desc.m_StencilFunc;
            dsaDesc.m_StencilMask = desc.m_StencilMask;
            dsaDesc.m_StencilFail = desc.m_StencilFail;
            dsaDesc.m_StencilPassDepthFail = desc.m_StencilPassDepthFail;
            dsaDesc.m_StencilPass = desc.m_StencilPass;
            m_DepthStencilAttachment = new GlDepthStencilAttachment(dsaDesc);
            glNamedFramebufferRenderbuffer(m_FrameBufferID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachment->getIdentifier());
        }

        SR_CORE_POSTCONDITION(glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer creation failed!");
    }

    GlFrameBuffer::~GlFrameBuffer() {
        glDeleteFramebuffers(1, &m_FrameBufferID);
    }

    void GlFrameBuffer::bind() {
        SR_CORE_PRECONDITION(glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Cannot bind the framebuffer because the framebuffer is not complete");

        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

        // call every time a different framebuffer is bound, this must happen because the viewport is not a part of the framebuffer state
        glViewport(m_XPos, m_YPos, m_Width, m_Height);

        if (m_DepthStencilAttachment){
            m_DepthStencilAttachment->bind();
        }


    }

    void GlFrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if (m_DepthStencilAttachment){
            m_DepthStencilAttachment->unbind();
        }
    }

    void GlFrameBuffer::clear() {
        for (int32 i = 0; i < m_ColorAttachments.size(); i++){
            glClearNamedFramebufferfv(m_FrameBufferID, GL_COLOR, i, m_ClearColor);
        }
        if (m_EnableDepthTest){
            float clearDepth = m_DepthStencilAttachment->getClearDepth();
            glClearNamedFramebufferfv(m_FrameBufferID, GL_DEPTH, 0, &clearDepth);
        }
        if (m_EnableStencilTest){
            uint32 clearStencil = m_DepthStencilAttachment->getClearStencil();
            glClearNamedFramebufferuiv(m_FrameBufferID, GL_STENCIL, 0, &clearStencil);
        }
    }


    void GlFrameBuffer::setPosition(int32 xPos, int32 yPos) {
        m_XPos = xPos;
        m_YPos = yPos;
    }

    void GlFrameBuffer::setSize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        for (const auto colorAttachment : m_ColorAttachments){
            colorAttachment->onResize(width, height);
        }
        if (m_DepthStencilAttachment){
            m_DepthStencilAttachment->onResize(width, height);
        }

    }


    GlDefaultFramebuffer::GlDefaultFramebuffer(const FrameBufferDesc &desc)
    : FrameBuffer(desc),
    m_EnableStencilTest(desc.m_EnableStencilTest),
    m_EnableDepthTest(desc.m_EnableDepthTest){

        if (desc.m_EnableDepthTest or desc.m_EnableStencilTest){
            DepthStencilAttachmentDesc dsaDesc;
            dsaDesc.m_Width = m_Width;
            dsaDesc.m_Height = m_Height;
            dsaDesc.m_EnableDepthTest = desc.m_EnableDepthTest;
            dsaDesc.m_DepthBufferReadOnly = desc.m_DepthBufferReadOnly;
            dsaDesc.m_DepthFunc = desc.m_DepthFunc;
            dsaDesc.m_EnableStencilTest = desc.m_EnableStencilTest;
            dsaDesc.m_StencilBufferReadOnly = desc.m_StencilBufferReadOnly;
            dsaDesc.m_StencilFunc = desc.m_StencilFunc;
            dsaDesc.m_StencilMask = desc.m_StencilMask;
            dsaDesc.m_StencilFail = desc.m_StencilFail;
            dsaDesc.m_StencilPassDepthFail = desc.m_StencilPassDepthFail;
            dsaDesc.m_StencilPass = desc.m_StencilPass;
            m_DepthStencilAttachment = new GlDefaultDepthStencilAttachment(dsaDesc);
        }

    }

    GlDefaultFramebuffer::~GlDefaultFramebuffer() {

    }

    void GlDefaultFramebuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // call every time a different framebuffer is bound, this must happen because the viewport is not a part of the framebuffer state
        glViewport(m_XPos, m_YPos, m_Width, m_Height);
        if (m_DepthStencilAttachment){
            m_DepthStencilAttachment->bind();
        }
    }

    void GlDefaultFramebuffer::unbind() {
        if (m_DepthStencilAttachment){
            m_DepthStencilAttachment->unbind();
        }
    }

    void GlDefaultFramebuffer::clear() {
        glClearNamedFramebufferfv(0, GL_COLOR, 0, m_ClearColor);
        if (m_EnableDepthTest){
            float clearDepth = m_DepthStencilAttachment->getClearDepth();
            glClearNamedFramebufferfv(0, GL_DEPTH, 0, &clearDepth);
        }
        if (m_EnableStencilTest){
            uint32 clearStencil = m_DepthStencilAttachment->getClearStencil();
            glClearNamedFramebufferuiv(0, GL_STENCIL, 0, &clearStencil);
        }
    }

    void GlDefaultFramebuffer::setPosition(int32 xPos, int32 yPos) {
        m_XPos = xPos;
        m_YPos = yPos;
    }

    void GlDefaultFramebuffer::setSize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
    }


}
