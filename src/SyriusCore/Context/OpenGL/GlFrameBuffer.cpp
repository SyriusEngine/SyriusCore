#include "GlFrameBuffer.hpp"

namespace Syrius{

    GlFrameBuffer::GlFrameBuffer(const FrameBufferDesc &desc)
    : FrameBuffer(desc),
    m_FrameBufferID(0),
      m_EnableStencilTest(desc.m_DepthStencilAttachment.m_EnableStencilTest),
      m_EnableDepthTest(desc.m_DepthStencilAttachment.m_EnableDepthTest){
        glCreateFramebuffers(1, &m_FrameBufferID);

        GLenum attachmentIndex = GL_COLOR_ATTACHMENT0;
        std::vector<GLenum> drawBuffers;
        for (const auto& colorAttachDesc : desc.m_ColorAttachments){
            SR_CORE_MESSAGE_ON_CONDITION((colorAttachDesc.m_Width != m_Width || colorAttachDesc.m_Height != m_Height), "Color attachment size does not match framebuffer size");

            auto colorAttachment = new GlColorAttachment(colorAttachDesc);
            m_ColorAttachments.push_back(colorAttachment);
            drawBuffers.push_back(attachmentIndex);
            glNamedFramebufferTexture(m_FrameBufferID, attachmentIndex, colorAttachment->getIdentifier(), 0);
            attachmentIndex++;
        }
        glNamedFramebufferDrawBuffers(m_FrameBufferID, drawBuffers.size(), &drawBuffers[0]);

        if (desc.m_DepthStencilAttachment.m_EnableDepthTest or desc.m_DepthStencilAttachment.m_EnableStencilTest){
            m_DepthStencilAttachment = new GlDepthStencilAttachment(desc.m_DepthStencilAttachment);
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
            glClearNamedFramebufferfv(m_FrameBufferID, GL_COLOR, i, m_ColorAttachments[i]->getClearColor());
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
    m_EnableStencilTest(desc.m_DepthStencilAttachment.m_EnableStencilTest),
    m_EnableDepthTest(desc.m_DepthStencilAttachment.m_EnableDepthTest){
        SR_CORE_PRECONDITION(desc.m_ColorAttachments.size() == 1, "Default framebuffer must have exactly one color attachment");

        m_ColorAttachments.push_back(new GlDefaultColorAttachment(desc.m_ColorAttachments[0]));

        if (desc.m_DepthStencilAttachment.m_EnableDepthTest or desc.m_DepthStencilAttachment.m_EnableStencilTest){
            m_DepthStencilAttachment = new GlDefaultDepthStencilAttachment(desc.m_DepthStencilAttachment);
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
        m_ColorAttachments[0]->clear();
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
