#include "GlFrameBuffer.hpp"

namespace Syrius{

    GLint checkFramebuffer(uint32 framebuffer){
        auto status = glCheckNamedFramebufferStatus(framebuffer, GL_FRAMEBUFFER);
        std::string errorStr;
        switch (status){
            case GL_FRAMEBUFFER_COMPLETE:                       errorStr = "GL_FRAMEBUFFER_COMPLETE"; break;
            case GL_FRAMEBUFFER_UNDEFINED:                      errorStr = "GL_FRAMEBUFFER_UNDEFINED"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:          errorStr = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:  errorStr = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:         errorStr = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:         errorStr = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
            case GL_FRAMEBUFFER_UNSUPPORTED:                    errorStr = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:         errorStr = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:       errorStr = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
            default: errorStr += "Unknown error"; break;
        }
        if (status != GL_FRAMEBUFFER_COMPLETE){
            SR_CORE_THROW("[GlFrameBuffer]: Framebuffer (%i) is not complete, error: %s", framebuffer, errorStr.c_str());
        }
        return status;
    }

    GlFrameBuffer::GlFrameBuffer(const ResourceView<FrameBufferLayout> &desc, const Resource<DeviceLimits>& deviceLimits) :
    FrameBuffer(desc, deviceLimits),
    m_FrameBufferID(0){
        for (const auto& viewDesc: desc->getViewportDesc()){
            auto viewport = new GlViewport(viewDesc, m_DeviceLimits);
            m_Viewports.emplace_back(viewport);
        }

        glCreateFramebuffers(1, &m_FrameBufferID);
        createColorAttachments(desc);
        createDepthStencilAttachment(desc);

        SR_CORE_POSTCONDITION(checkFramebuffer(m_FrameBufferID) == GL_FRAMEBUFFER_COMPLETE, "[GlFrameBuffer]: Framebuffer (%i) creation failed!", m_FrameBufferID);
    }

    GlFrameBuffer::~GlFrameBuffer() {
        glDeleteFramebuffers(1, &m_FrameBufferID);
    }

    void GlFrameBuffer::bind() {
        SR_CORE_PRECONDITION(glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "[GlFrameBuffer]: Framebuffer (%i) cannot be bound because it is incomplete", m_FrameBufferID);

        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

        for (const auto& viewport: m_Viewports){
            viewport->bind();
        }
        m_DepthStencilAttachment->bind();

    }

    void GlFrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_DepthStencilAttachment->unbind();
    }

    void GlFrameBuffer::createColorAttachments(const ResourceView<FrameBufferLayout> &desc) {
        std::vector<GLenum> drawBuffers;
        uint32 colorAttachmentIndex = 0;
        // 2D color attachments
        for (const auto& attachDesc: desc->getColorAttachmentDesc()){
            auto attachment = new GlColorAttachment(attachDesc, m_DeviceLimits, m_FrameBufferID, colorAttachmentIndex);
            m_ColorAttachments.emplace_back(attachment);
            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + colorAttachmentIndex);
            colorAttachmentIndex++;
        }
        // Cube color attachments
//        for (const auto& attachDesc: desc->getCubeColorAttachmentDesc()){
//            auto attachment = new GlCubeColorAttachment(attachDesc, m_DeviceLimits, m_FrameBufferID, colorAttachmentIndex);
//            m_CubeColorAttachments.emplace_back(attachment);
//            auto identifier = attachment->getIdentifier();
//            for (uint32 i = 0; i < 6; i++){
//                glNamedFramebufferTexture(m_FrameBufferID, attachmentIndex, attachment->getIdentifier(), 0);
//                drawBuffers.push_back(attachmentIndex);
//                attachmentIndex++;
//                colorAttachmentIndex++;
//            }
//        }

        glNamedFramebufferDrawBuffers(m_FrameBufferID, drawBuffers.size(), &drawBuffers[0]);

    }

    void GlFrameBuffer::createDepthStencilAttachment(const ResourceView<FrameBufferLayout> &desc) {
        if (!desc->getDepthStencilAttachmentDesc().empty()){
            auto dsaDesc = desc->getDepthStencilAttachmentDesc().back();
            GlDepthStencilAttachment* attachment;
            if (dsaDesc.enableShaderAccess){
                attachment = new GlDepthStencilAttachmentTexture(dsaDesc, m_DeviceLimits, m_FrameBufferID);
            }
            else{
                attachment = new GlDepthStencilAttachmentRenderBuffer(dsaDesc, m_DeviceLimits, m_FrameBufferID);
            }
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(attachment);
        }
        else{
            // create a dummy depth stencil attachment such that the object exists but does nothing
            DepthStencilAttachmentDesc dummyDesc;
            dummyDesc.width = desc->getViewportDesc().back().width;
            dummyDesc.height = desc->getViewportDesc().back().height;
            dummyDesc.enableDepthTest = false;
            dummyDesc.enableStencilTest = false;
            dummyDesc.enableShaderAccess = false;
            auto df = new GlDepthStencilAttachmentRenderBuffer(dummyDesc, m_DeviceLimits, 0);
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(df);
        }
    }

    GlDefaultFrameBuffer::GlDefaultFrameBuffer(const ResourceView<FrameBufferLayout> &desc, const Resource<DeviceLimits>& deviceLimits) :
    FrameBuffer(desc, deviceLimits) {
        auto viewport = new GlViewport(desc->getViewportDesc().back(), m_DeviceLimits);
        m_Viewports.emplace_back(viewport);

        auto attachment = new GlDefaultColorAttachment(desc->getColorAttachmentDesc().back(), m_DeviceLimits);
        m_ColorAttachments.emplace_back(attachment);

        if (!desc->getDepthStencilAttachmentDesc().empty()){
            if (desc->getDepthStencilAttachmentDesc().size() > 1){
                SR_CORE_MESSAGE("The default framebuffer cannot have more than one depth stencil attachment, only the last one will be used");
            }
            auto df = new GlDefaultDepthStencilAttachment(desc->getDepthStencilAttachmentDesc().back(), m_DeviceLimits);
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(df);
        }
        else{
            // create a dummy depth stencil attachment such that the object exists but does nothing
            DepthStencilAttachmentDesc dummyDesc;
            dummyDesc.width = desc->getViewportDesc().back().width;
            dummyDesc.height = desc->getViewportDesc().back().height;
            dummyDesc.enableDepthTest = false;
            dummyDesc.enableStencilTest = false;
            auto df = new GlDefaultDepthStencilAttachment(dummyDesc, m_DeviceLimits);
            m_DepthStencilAttachment = Resource<DepthStencilAttachment>(df);
        }

    }

    GlDefaultFrameBuffer::~GlDefaultFrameBuffer() = default;

    void GlDefaultFrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        for (const auto& viewport: m_Viewports){
            viewport->bind();
        }
        m_DepthStencilAttachment->bind();
    }

    void GlDefaultFrameBuffer::unbind() {
        m_DepthStencilAttachment->unbind();
    }

}
