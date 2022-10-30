#pragma once

#include "../../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "GlUtils.hpp"
#include "GlColorAttachment.hpp"
#include "GlDepthStencilAttachment.hpp"

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

        bool m_EnableDepthTest;
        bool m_EnableStencilTest;
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
        bool m_EnableDepthTest;
        bool m_EnableStencilTest;

    };
    
}

