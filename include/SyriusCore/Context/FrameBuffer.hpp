#pragma once

#include "Utils.hpp"
#include "FrameBufferLayout.hpp"
#include "Viewport.hpp"
#include "ColorAttachment.hpp"
#include "DepthStencilAttachment.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    class SR_CORE_API FrameBuffer{
    public:
        FrameBuffer(const ResourceView<FrameBufferLayout>& desc, const UP<DeviceLimits>& deviceLimits);

        virtual ~FrameBuffer();

        virtual void bind() = 0;

        virtual void unbind() = 0;

        void clear();

        virtual void onResize(u32 width, u32 height);

        void enableDepthTest(bool enable);

        void setDepthFunc(SR_COMPARISON_FUNC func);

        void setDepthMask(SR_DEPTH_MASK mask);

        ResourceView<Viewport> getViewport(u32 index = 0);

        [[nodiscard]] ResourceView<ColorAttachment> getColorAttachment(u32 index);

        ResourceView<DepthStencilAttachment> getDepthStencilAttachment();

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;

        std::vector<UP<Viewport>> m_Viewports;
        std::vector<UP<ColorAttachment>> m_ColorAttachments;
        UP<DepthStencilAttachment> m_DepthStencilAttachment;

    };


}

