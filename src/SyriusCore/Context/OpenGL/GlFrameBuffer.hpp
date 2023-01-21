#pragma once

#include "../../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "GlUtils.hpp"
#include "GlViewport.hpp"
#include "GlColorAttachment.hpp"
#include "GlDepthStencilAttachment.hpp"

namespace Syrius{

    class GlFrameBuffer : public FrameBuffer{
    public:
        explicit GlFrameBuffer(const ResourceView<FrameBufferDescription>& desc);

        ~GlFrameBuffer() override;

        void bind() override;

        void unbind() override;

    private:
        uint32 m_FrameBufferID;

    };

    class GlDefaultFrameBuffer : public FrameBuffer{
    public:
        explicit GlDefaultFrameBuffer(const ResourceView<FrameBufferDescription>& desc);

        ~GlDefaultFrameBuffer() override;

        void bind() override;

        void unbind() override;
    };

}