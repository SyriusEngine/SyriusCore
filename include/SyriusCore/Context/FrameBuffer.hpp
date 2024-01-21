#pragma once

#include "Utils.hpp"
#include "FramebufferDescription.hpp"
#include "Viewport.hpp"
#include "ColorAttachment.hpp"
#include "DepthStencilAttachment.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    class SR_CORE_API FrameBuffer{
    public:
        FrameBuffer(const ResourceView<FrameBufferDescription>& desc, const Resource<DeviceLimits>& deviceLimits);

        virtual ~FrameBuffer();

        virtual void bind() = 0;

        virtual void unbind() = 0;

        void clear();

        virtual void onResize(uint32 width, uint32 height);

        void enableDepthTest(bool enable);

        void setDepthFunc(SR_COMPARISON_FUNC func);

        void setDepthMask(SR_DEPTH_MASK mask);

        ResourceView<Viewport> getViewport(uint32 index = 0);

        [[nodiscard]] ResourceView<ColorAttachment> getColorAttachment(uint32 index);

        ResourceView<DepthStencilAttachment> getDepthStencilAttachment();

    protected:
        const Resource<DeviceLimits>& m_DeviceLimits;

        std::vector<Resource<Viewport>> m_Viewports;
        std::vector<Resource<ColorAttachment>> m_ColorAttachments;
        Resource<DepthStencilAttachment> m_DepthStencilAttachment;

    };


}

