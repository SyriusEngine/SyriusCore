#pragma once

#include "Utils.hpp"
#include "ColorAttachment.hpp"

namespace Syrius{

    struct SR_API FrameBufferDesc{
        // viewport desc
        uint32 m_Width                  = SR_DEFAULT_WIDTH;
        uint32 m_Height                 = SR_DEFAULT_HEIGHT;
        int32 m_XPos                    = 0;
        int32 m_YPos                    = 0;
        float m_MinDepth                = 0.0f;
        float m_MaxDepth                = 1.0f;
        // clear parameters
        float m_ClearColor[4]           = {0.8f, 0.2f, 0.3f, 1.0f};
        float m_ClearDepth              = 1.0f;
        // depth parameters
        bool m_EnableDepthTest          = false;
        bool m_DepthTestReadOnly        = false;
        SR_COMPARISON_FUNC m_DepthFunc  = SR_COMPARISON_FUNC_LESS;
        // stencil parameters
        bool m_EnableStencilTest        = false;
        bool m_StencilTestReadOnly      = false;
        SR_COMPARISON_FUNC m_StencilFunc = SR_COMPARISON_FUNC_ALWAYS;
        uint32 m_StencilRef             = 0;
        uint32 m_StencilMask            = 0xFFFFFFFF;

        // color attachment desc
        std::vector<ColorAttachmentDesc> m_ColorAttachments = {ColorAttachmentDesc()};
    };

    class SR_API FrameBuffer{
    public:
        explicit FrameBuffer(const FrameBufferDesc& desc);

        virtual ~FrameBuffer();

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual void setClearColor(float red, float green, float blue, float alpha) = 0;

        virtual void setPosition(int32 xPos, int32 yPos) = 0;

        virtual void setDepthFunc(SR_COMPARISON_FUNC func) = 0;

        virtual void onResize(uint32 width, uint32 height) = 0;

        virtual void clear() = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] int32 getXPos() const;

        [[nodiscard]] int32 getYPos() const;

        [[nodiscard]] float getMinDepth() const;

        [[nodiscard]] float getMaxDepth() const;

        [[nodiscard]] bool isDepthTestEnabled() const;

        [[nodiscard]] SR_COMPARISON_FUNC getDepthFunc() const;

        [[nodiscard]] ColorAttachment* getColorAttachment(uint32 index) const;

    protected:
        uint32 m_Width;
        uint32 m_Height;
        int32 m_XPos;
        int32 m_YPos;

        float m_MinDepth;
        float m_MaxDepth;
        bool m_EnableDepthTest;
        SR_COMPARISON_FUNC m_DepthFunc;

        float m_ClearColor[4];

        std::vector<ColorAttachment*> m_ColorAttachments;

    };

}

