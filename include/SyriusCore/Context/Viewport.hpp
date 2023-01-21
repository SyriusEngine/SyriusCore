#pragma once

#include "../Utils/Resource.hpp"
#include "Utils.hpp"

namespace Syrius{

    struct SR_CORE_API ViewportDesc{
        uint32 m_Width          = SR_DEFAULT_WIDTH;
        uint32 m_Height         = SR_DEFAULT_HEIGHT;
        uint32 m_XPos           = 0;
        uint32 m_YPos           = 0;
        float m_MinDepth        = 0.0f;
        float m_MaxDepth        = 1.0f;
    };

    class SR_CORE_API Viewport{
    public:
        explicit Viewport(const ViewportDesc& desc);

        virtual ~Viewport();

        virtual void bind() = 0;

        void setSize(uint32 width, uint32 height);

        void setPos(uint32 x, uint32 y);

        void setDepthRange(float minDepth, float maxDepth);

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] uint32 getXPos() const;

        [[nodiscard]] uint32 getYPos() const;

        [[nodiscard]] float getMinDepth() const;

        [[nodiscard]] float getMaxDepth() const;

    protected:
        uint32 m_Width;
        uint32 m_Height;
        uint32 m_XPos;
        uint32 m_YPos;
        float m_MinDepth;
        float m_MaxDepth;
    };

}
