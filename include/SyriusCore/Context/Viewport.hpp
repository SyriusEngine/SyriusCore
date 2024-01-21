#pragma once

#include "../Utils/Resource.hpp"
#include "Utils.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API ViewportDesc{
        uint32 width          = SR_DEFAULT_WIDTH;
        uint32 height         = SR_DEFAULT_HEIGHT;
        uint32 xPos           = 0;
        uint32 yPos           = 0;
        float minDepth        = 0.0f;
        float maxDepth        = 1.0f;
    };

    class SR_CORE_API Viewport{
    public:
        Viewport(const ViewportDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        virtual ~Viewport();

        virtual void bind() = 0;

        void onResize(uint32 width, uint32 height);

        void setPos(uint32 x, uint32 y);

        void setDepthRange(float minDepth, float maxDepth);

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] uint32 getXPos() const;

        [[nodiscard]] uint32 getYPos() const;

        [[nodiscard]] float getMinDepth() const;

        [[nodiscard]] float getMaxDepth() const;

    protected:
        const Resource<DeviceLimits>& m_DeviceLimits;

        uint32 m_Width;
        uint32 m_Height;
        uint32 m_XPos;
        uint32 m_YPos;
        float m_MinDepth;
        float m_MaxDepth;
    };

}
