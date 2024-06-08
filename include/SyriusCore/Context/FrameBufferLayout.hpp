#pragma once

#include "Viewport.hpp"
#include "ColorAttachment.hpp"
#include "DepthStencilAttachment.hpp"
#include "CubeColorAttachment.hpp"

namespace Syrius{

    class SR_CORE_API FrameBufferLayout{
    public:
        explicit FrameBufferLayout(const Resource<DeviceLimits>& deviceLimits);

        ~FrameBufferLayout();

        void addViewportDesc(const ViewportDesc& desc);

        void addColorAttachmentDesc(const ColorAttachmentDesc& desc);

        void addDepthStencilAttachmentDesc(const DepthStencilAttachmentDesc& desc);

        void addCubeColorAttachmentDesc(const CubeColorAttachmentDesc& desc);

        void configure();

        [[nodiscard]] const std::vector<ViewportDesc>& getViewportDesc() const;

        [[nodiscard]] const std::vector<ColorAttachmentDesc>& getColorAttachmentDesc() const;

        [[nodiscard]] const std::vector<DepthStencilAttachmentDesc>& getDepthStencilAttachmentDesc() const;

        [[nodiscard]] const std::vector<CubeColorAttachmentDesc>& getCubeColorAttachmentDesc() const;

    private:
        const Resource<DeviceLimits>& m_DeviceLimits;

        std::vector<ViewportDesc> m_ViewportDesc;
        std::vector<ColorAttachmentDesc> m_ColorAttachmentDesc;
        std::vector<DepthStencilAttachmentDesc> m_DepthStencilAttachmentDesc; // only one depth stencil attachment is allowed, so the last added will be used
        std::vector<CubeColorAttachmentDesc> m_CubeColorAttachmentDesc;

    };

}
