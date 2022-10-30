#pragma once

#include "Utils.hpp"
#include "ColorAttachment.hpp"
#include "DepthStencilAttachment.hpp"

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
        uint32 m_ClearStencil           = 0;

        // depth parameters
        bool m_EnableDepthTest          = false;
        bool m_DepthBufferReadOnly      = false;
        SR_COMPARISON_FUNC m_DepthFunc  = SR_COMPARISON_FUNC_LESS;

        // stencil parameters
        bool m_EnableStencilTest        = false;
        bool m_StencilBufferReadOnly    = false;
        SR_COMPARISON_FUNC m_StencilFunc = SR_COMPARISON_FUNC_ALWAYS;
        uint32 m_StencilMask            = 0xFFFFFFFF;
        SR_STENCIL_FUNC m_StencilFail   = SR_STENCIL_FUNC_KEEP;
        SR_STENCIL_FUNC m_StencilPass   = SR_STENCIL_FUNC_KEEP;
        SR_STENCIL_FUNC m_StencilPassDepthFail = SR_STENCIL_FUNC_KEEP;

        // color attachment desc
        std::vector<ColorAttachmentDesc> m_ColorAttachments = {ColorAttachmentDesc()};
    };

    class SR_API FrameBuffer{
    public:
        explicit FrameBuffer(const FrameBufferDesc& desc);

        virtual ~FrameBuffer();

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual void clear() = 0;

        virtual void setPosition(int32 xPos, int32 yPos) = 0;

        virtual void setSize(uint32 width, uint32 height) = 0;

        void setClearColor(float red, float green, float blue, float alpha);

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] int32 getXPos() const;

        [[nodiscard]] int32 getYPos() const;

        [[nodiscard]] float getMinDepth() const;

        [[nodiscard]] float getMaxDepth() const;

        [[nodiscard]] ColorAttachment* getColorAttachment(uint32 index) const;

    protected:
        uint32 m_Width;
        uint32 m_Height;
        int32 m_XPos;
        int32 m_YPos;
        float m_MinDepth;
        float m_MaxDepth;

        float m_ClearColor[4];
        float m_ClearDepth;
        uint32 m_ClearStencil;

        std::vector<ColorAttachment*> m_ColorAttachments;
        DepthStencilAttachment* m_DepthStencilAttachment;

    };

}

