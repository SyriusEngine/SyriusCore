#pragma once

#include "Utils.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API ViewportDesc{
        u32 width          = SR_DEFAULT_WIDTH;
        u32 height         = SR_DEFAULT_HEIGHT;
        u32 xPos           = 0;
        u32 yPos           = 0;
        float minDepth        = 0.0f;
        float maxDepth        = 1.0f;
    };

    class SR_CORE_API Viewport{
    public:
        Viewport(const ViewportDesc& desc, const UP<DeviceLimits>& deviceLimits);

        virtual ~Viewport();

        virtual void bind() = 0;

        void onResize(u32 width, u32 height);

        void setPos(u32 x, u32 y);

        void setDepthRange(float minDepth, float maxDepth);

        [[nodiscard]] u32 getWidth() const;

        [[nodiscard]] u32 getHeight() const;

        [[nodiscard]] u32 getXPos() const;

        [[nodiscard]] u32 getYPos() const;

        [[nodiscard]] float getMinDepth() const;

        [[nodiscard]] float getMaxDepth() const;

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;

        u32 m_Width;
        u32 m_Height;
        u32 m_XPos;
        u32 m_YPos;
        float m_MinDepth;
        float m_MaxDepth;
    };

}
