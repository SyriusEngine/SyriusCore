#include "GlFrameBuffer.hpp"

namespace Syrius{

    GLint checkFramebuffer(uint32 framebuffer){
        auto status = glCheckNamedFramebufferStatus(framebuffer, GL_FRAMEBUFFER);
        std::string message = "openGL Framebuffer error: ";
        switch (status){
            case GL_FRAMEBUFFER_COMPLETE: message += "GL_FRAMEBUFFER_COMPLETE"; break;
            case GL_FRAMEBUFFER_UNDEFINED: message += "GL_FRAMEBUFFER_UNDEFINED"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: message += "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: message += "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: message += "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: message += "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
            case GL_FRAMEBUFFER_UNSUPPORTED: message += "GL_FRAMEBUFFER_UNSUPPORTED"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: message += "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: message += "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
            default: message += "Unknown error"; break;
        }
        if (status != GL_FRAMEBUFFER_COMPLETE){
            SR_CORE_EXCEPTION(message);
        }
        return status;
    }

    GlFrameBuffer::GlFrameBuffer(const ResourceView<FrameBufferDescription> &desc) :
    FrameBuffer(desc),
    m_FrameBufferID(0){
        for (const auto& viewDesc: desc->getViewportDesc()){
            auto viewport = new GlViewport(viewDesc);
            m_Viewports.emplace_back(viewport);
        }

        glCreateFramebuffers(1, &m_FrameBufferID);

        GLenum attachmentIndex = GL_COLOR_ATTACHMENT0;
        std::vector<GLenum> drawBuffers;
        uint32 i = 0;
        for (const auto& attachDesc: desc->getColorAttachmentDesc()){
            auto attachment = new GlColorAttachment(attachDesc);
            m_ColorAttachments.emplace_back(attachment);
            drawBuffers.push_back(attachmentIndex);
            glNamedFramebufferTexture(m_FrameBufferID, attachmentIndex + i, attachment->getIdentifier(), 0);
            i++;
        }
        glNamedFramebufferDrawBuffers(m_FrameBufferID, drawBuffers.size(), &drawBuffers[0]);

        if (!desc->getDepthStencilAttachmentDesc().empty()){
            auto attachment = new GlDepthStencilAttachment(desc->getDepthStencilAttachmentDesc().back(), m_FrameBufferID);
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(attachment);
            glNamedFramebufferRenderbuffer(m_FrameBufferID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, attachment->getIdentifier());

        }

        SR_CORE_POSTCONDITION(checkFramebuffer(m_FrameBufferID) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer creation failed!");
    }

    GlFrameBuffer::~GlFrameBuffer() {
        glDeleteFramebuffers(1, &m_FrameBufferID);
    }

    void GlFrameBuffer::bind() {
        SR_CORE_PRECONDITION(glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Cannot bind the framebuffer because the framebuffer is not complete");

        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

        for (const auto& viewport: m_Viewports){
            viewport->bind();
        }
        if (m_DepthStencilAttachment.isValid()){
            m_DepthStencilAttachment->bind();
        }

    }

    void GlFrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if (m_DepthStencilAttachment.isValid()){
            m_DepthStencilAttachment->unbind();
        }
    }

    GlDefaultFrameBuffer::GlDefaultFrameBuffer(const ResourceView<FrameBufferDescription> &desc) :
    FrameBuffer(desc) {
        auto viewport = new GlViewport(desc->getViewportDesc().back());
        m_Viewports.emplace_back(viewport);

        auto attachment = new GlDefaultColorAttachment(desc->getColorAttachmentDesc().back());
        m_ColorAttachments.emplace_back(attachment);

        if (!desc->getDepthStencilAttachmentDesc().empty()){
            auto df = new GlDepthStencilAttachment(desc->getDepthStencilAttachmentDesc().back(), 0);
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(df);

        }

    }

    GlDefaultFrameBuffer::~GlDefaultFrameBuffer() {

    }

    void GlDefaultFrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        for (const auto& viewport: m_Viewports){
            viewport->bind();
        }

        if (m_DepthStencilAttachment.isValid()){
            m_DepthStencilAttachment->bind();
        }
    }

    void GlDefaultFrameBuffer::unbind() {
        // impossible to unbind the default framebuffer
        if (m_DepthStencilAttachment.isValid()){
            m_DepthStencilAttachment->unbind();
        }
    }

}
