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

        void setClearColor(float red, float green, float blue, float alpha) override;

        void setPosition(int32 xPos, int32 yPos) override;

        void setDepthFunc(SR_COMPARISON_FUNC func) override;

        void onResize(uint32 width, uint32 height) override;

        void clear() override;

    private:
        uint32 m_FrameBufferID;
        GLbitfield m_ClearMask;
        GLenum m_GlDepthFunc;

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

        void setDepthFunc(SR_COMPARISON_FUNC func) override;

        void onResize(uint32 width, uint32 height) override;

        void clear() override;

    private:
        GLbitfield m_ClearMask;
        GLenum m_GlDepthFunc;

    };
    
}

