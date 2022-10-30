#include "GlFrameBuffer.hpp"

namespace Syrius{

    GlFrameBuffer::GlFrameBuffer(const FrameBufferDesc &desc)
    : FrameBuffer(desc),
    m_FrameBufferID(0),
    m_GlDepthFunc(getGlComparisonFunc(desc.m_DepthFunc)),
    m_GlStencilFunc(getGlComparisonFunc(desc.m_StencilFunc)),
    m_RenderBuffer(nullptr){
        SR_CORE_PRECONDITION(!desc.m_ColorAttachments.empty(), "Framebuffer must have at least one color attachment!");

        glCreateFramebuffers(1, &m_FrameBufferID);

        GLenum attachmentIndex = GL_COLOR_ATTACHMENT0;
        for (const auto& colorAttachmentDesc : desc.m_ColorAttachments){
            auto colorAttachment = new GlColorAttachment(colorAttachmentDesc);
            m_ColorAttachments.push_back(colorAttachment);
            glNamedFramebufferTexture(m_FrameBufferID, attachmentIndex, colorAttachment->getIdentifier(), 0);
            attachmentIndex++;
        }

//        if (m_EnableDepthTest or m_EnableStencilTest){
//            m_RenderBuffer = new GlRenderBuffer(m_Width, m_Height);
//            glNamedFramebufferRenderbuffer(m_FrameBufferID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer->getIdentifier());
//        }


        SR_CORE_POSTCONDITION(glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer creation failed!");
    }

    GlFrameBuffer::~GlFrameBuffer() {
        delete m_RenderBuffer;

        glDeleteFramebuffers(1, &m_FrameBufferID);
    }

    void GlFrameBuffer::bind() {
        SR_CORE_PRECONDITION(glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Cannot bind the framebuffer because the framebuffer is not complete");

        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

        // call every time a different framebuffer is bound, this must happen because the viewport is not a part of the framebuffer state
        glViewport(m_XPos, m_YPos, m_Width, m_Height);

//        if (m_EnableDepthTest or m_EnableStencilTest){
//            m_RenderBuffer->bind();
//            if (m_EnableDepthTest){
//                glEnable(GL_DEPTH_TEST);
//                glDepthFunc(m_GlDepthFunc);
//                glDepthMask(!m_DepthBufferReadOnly);
//            }
//            else{
//                glDisable(GL_DEPTH_TEST);
//            }
//
//            if (m_EnableStencilTest){
//                glEnable(GL_STENCIL_TEST);
//                glStencilFunc(m_GlStencilFunc, m_ClearStencil, m_StencilMask);
//                glStencilMask(!m_StencilBufferReadOnly);
//            }
//            else{
//                glDisable(GL_STENCIL_TEST);
//            }
//        }

    }

    void GlFrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GlFrameBuffer::clear() {
        for (int32 i = 0; i < m_ColorAttachments.size(); i++){
            glClearNamedFramebufferfv(m_FrameBufferID, GL_COLOR, i, m_ClearColor);
        }
//        if (m_EnableDepthTest){
//            glClearNamedFramebufferfv(m_FrameBufferID, GL_DEPTH, 0, &m_ClearDepth);
//        }
//        if (m_EnableStencilTest){
//            glClearNamedFramebufferuiv(m_FrameBufferID, GL_STENCIL, 0, &m_ClearStencil);
//        }
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
        if (m_RenderBuffer){
            m_RenderBuffer->onResize(width, height);
        }

    }


    GlDefaultFramebuffer::GlDefaultFramebuffer(const FrameBufferDesc &desc)
    : FrameBuffer(desc),
    m_GlDepthFunc(getGlComparisonFunc(desc.m_DepthFunc)),
    m_GlStencilFunc(getGlComparisonFunc(desc.m_StencilFunc)){

    }

    GlDefaultFramebuffer::~GlDefaultFramebuffer() {

    }

    void GlDefaultFramebuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // call every time a different framebuffer is bound, this must happen because the viewport is not a part of the framebuffer state
        glViewport(m_XPos, m_YPos, m_Width, m_Height);

//        if (m_EnableDepthTest){
//            glEnable(GL_DEPTH_TEST);
//            glDepthFunc(m_GlDepthFunc);
//            glDepthMask(!m_DepthBufferReadOnly);
//        }
//        else{
//            glDisable(GL_DEPTH_TEST);
//        }
//
//        if (m_EnableStencilTest){
//            glEnable(GL_STENCIL_TEST);
//            glStencilFunc(m_GlStencilFunc, m_ClearStencil, m_StencilMask);
//            glStencilMask(!m_StencilBufferReadOnly);
//        }
//        else{
//            glDisable(GL_STENCIL_TEST);
//        }
    }

    void GlDefaultFramebuffer::unbind() {

    }

    void GlDefaultFramebuffer::clear() {
        glClearNamedFramebufferfv(0, GL_COLOR, 0, m_ClearColor);
//        if (m_EnableDepthTest){
//            glClearNamedFramebufferfv(0, GL_DEPTH, 0, &m_ClearDepth);
//        }
//        if (m_EnableStencilTest){
//            glClearNamedFramebufferuiv(0, GL_STENCIL, 0, &m_ClearStencil);
//        }
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
