#pragma once

#include "../../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "GlUtils.hpp"
#include "GlViewport.hpp"
#include "GlColorAttachment.hpp"
#include "GlDepthStencilAttachment.hpp"

namespace Syrius{

    class GlFrameBuffer : public FrameBuffer{
    public:
        GlFrameBuffer(const ResourceView<FrameBufferLayout>& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlFrameBuffer() override;

        void bind() override;

        void unbind() override;

    private:

        void createColorAttachments(const ResourceView<FrameBufferLayout>& desc);

        void createDepthStencilAttachment(const ResourceView<FrameBufferLayout>& desc);

    private:
        u32 m_FrameBufferID;

    };

    class GlDefaultFrameBuffer : public FrameBuffer{
    public:
        GlDefaultFrameBuffer(const ResourceView<FrameBufferLayout>& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlDefaultFrameBuffer() override;

        void bind() override;

        void unbind() override;
    };

}