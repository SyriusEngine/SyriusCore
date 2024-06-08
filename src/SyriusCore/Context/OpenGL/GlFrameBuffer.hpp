#pragma once

#include "../../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "GlUtils.hpp"
#include "GlViewport.hpp"
#include "GlColorAttachment.hpp"
#include "GlDepthStencilAttachment.hpp"
#include "GlCubeColorAttachment.hpp"

namespace Syrius{

    class GlFrameBuffer : public FrameBuffer{
    public:
        GlFrameBuffer(const ResourceView<FrameBufferLayout>& desc, const Resource<DeviceLimits>& deviceLimits);

        ~GlFrameBuffer() override;

        void bind() override;

        void unbind() override;

    private:
        uint32 m_FrameBufferID;

    };

    class GlDefaultFrameBuffer : public FrameBuffer{
    public:
        GlDefaultFrameBuffer(const ResourceView<FrameBufferLayout>& desc, const Resource<DeviceLimits>& deviceLimits);

        ~GlDefaultFrameBuffer() override;

        void bind() override;

        void unbind() override;
    };

}