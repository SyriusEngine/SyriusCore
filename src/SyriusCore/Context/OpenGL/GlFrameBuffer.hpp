#pragma once

#include "../../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "GlUtils.hpp"
#include "Internal/GlRenderBuffer.hpp"
#include "GlColorAttachment.hpp"

namespace Syrius{
    
    class SR_API GlFrameBuffer : public FrameBuffer{
    public:
        explicit GlFrameBuffer(const FrameBufferDesc& desc);
        
        ~GlFrameBuffer() override;

        void bind() override;

        void unbind() override;

        void clear() override;

        void setPosition(int32 xPos, int32 yPos) override;

        void setSize(uint32 width, uint32 height) override;

    private:
        uint32 m_FrameBufferID;
        GLenum m_GlDepthFunc;
        GLenum m_GlStencilFunc;

        GlRenderBuffer* m_RenderBuffer;
        
    };


    class GlDefaultFramebuffer: public FrameBuffer{
    public:
        explicit GlDefaultFramebuffer(const FrameBufferDesc& desc);

        ~GlDefaultFramebuffer() override;

        void bind() override;

        void unbind() override;

        void clear() override;

        void setPosition(int32 xPos, int32 yPos) override;

        void setSize(uint32 width, uint32 height) override;


    private:
        GLenum m_GlDepthFunc;
        GLenum m_GlStencilFunc;

    };
    
}

