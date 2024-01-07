#pragma once

#include "Utils.hpp"
#include "FramebufferDescription.hpp"
#include "Viewport.hpp"
#include "ColorAttachment.hpp"
#include "DepthStencilAttachment.hpp"

namespace Syrius{

    class SR_CORE_API FrameBuffer{
    public:
        explicit FrameBuffer(const ResourceView<FrameBufferDescription>& desc);

        virtual ~FrameBuffer();

        virtual void bind() = 0;

        virtual void unbind() = 0;

        void clear();

        virtual void onResize(uint32 width, uint32 height);

        void enableDepthTest(bool enable);

        ResourceView<Viewport> getViewport(uint32 index = 0);

        [[nodiscard]] ResourceView<ColorAttachment> getColorAttachment(uint32 index);

        ResourceView<DepthStencilAttachment> getDepthStencilAttachment();

    protected:
        std::vector<Resource<Viewport>> m_Viewports;
        std::vector<Resource<ColorAttachment>> m_ColorAttachments;
        Resource<DepthStencilAttachment> m_DepthStencilAttachment;

    };


}

