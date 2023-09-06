#pragma once

#include "../Utils/Resource.hpp"
#include "Utils.hpp"

namespace Syrius{

    struct SR_CORE_API ViewportDesc{
        uint32 width          = SR_DEFAULT_WIDTH;
        uint32 height         = SR_DEFAULT_HEIGHT;
        uint32 xPos           = 0;
        uint32 yPos           = 0;
        float minDepth        = 0.0f;
        float maxDepth        = 1.0f;
        bool enableAutoResize = true; // whenever the window is resized, this attachment will be resized as well
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

    private:

        friend class FrameBuffer;
        friend class D3D11FrameBuffer;

        void onResize(uint32 width, uint32 height); // called by the framebuffer when its owning window is resized

    protected:
        uint32 m_Width;
        uint32 m_Height;
        uint32 m_XPos;
        uint32 m_YPos;
        float m_MinDepth;
        float m_MaxDepth;

        bool m_EnableAutoResize;
    };

}
