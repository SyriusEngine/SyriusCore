#pragma once

#include "../../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "GlUtils.hpp"
#include "GlRenderTarget.hpp"

namespace Syrius{
    
    class SR_API GlFrameBuffer : public FrameBuffer{
    public:
        explicit GlFrameBuffer(const FrameBufferDesc& desc);
        
        ~GlFrameBuffer() override;

        void bind() override;

        void unbind() override;

        void setClearColor(float red, float green, float blue, float alpha) override;

        void setPosition(int32 xPos, int32 yPos) override;

        void setDepthFunc(SR_DEPTH_FUNC func) override;

        void onResize(uint32 width, uint32 height) override;

        void enableDepthTest() override;

        void disableDepthTest() override;

        void clear() override;

        void bindColorAttachment(uint32 slot, uint32 attachmentIndex) override;

        Image* readColorAttachment(uint32 attachmentIndex) override;

        uint64 getColorAttachmentIdentifier(uint32 attachmentIndex) override;


    private:
        uint32 m_FrameBufferID;
        GLbitfield m_ClearMask;
        GLenum m_GlDepthFunc;

        std::vector<GlColorAttachment*> m_ColorAttachments;
        GlRenderBuffer* m_RenderBuffer;
        
    };


    class GlDefaultFramebuffer: public FrameBuffer{
    public:
        explicit GlDefaultFramebuffer(const FrameBufferDesc& desc);

        ~GlDefaultFramebuffer() override;

        void bind() override;

        void unbind() override;

        void setClearColor(float red, float green, float blue, float alpha) override;

        void setPosition(int32 xPos, int32 yPos) override;

        void setDepthFunc(SR_DEPTH_FUNC func) override;

        void onResize(uint32 width, uint32 height) override;

        void enableDepthTest() override;

        void disableDepthTest() override;

        void clear() override;

        void bindColorAttachment(uint32 slot, uint32 attachmentIndex) override;

        Image* readColorAttachment(uint32 attachmentIndex) override;

        uint64 getColorAttachmentIdentifier(uint32 attachmentIndex) override;

    private:
        GLbitfield m_ClearMask;
        GLenum m_GlDepthFunc;

    };
    
}

