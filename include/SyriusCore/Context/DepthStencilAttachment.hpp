#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct SR_API DepthStencilAttachmentDesc{
        uint32 m_Width          = SR_DEFAULT_WIDTH;
        uint32 m_Height         = SR_DEFAULT_HEIGHT;

        float m_ClearDepth      = 1.0f;
        uint32 m_ClearStencil   = 0;

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
    };

    class SR_API DepthStencilAttachment{
    public:
        explicit DepthStencilAttachment(const DepthStencilAttachmentDesc& desc);

        virtual ~DepthStencilAttachment();

        virtual void bind() = 0;

        virtual void bindAsTexture(uint32 slot) = 0;

        virtual void unbind() = 0;

        virtual void clear() = 0;

        virtual void onResize(uint32 width, uint32 height) = 0;

        virtual void setDepthFunc(SR_COMPARISON_FUNC func) = 0;

        virtual void setDepthBufferReadOnly(bool readOnly) = 0;

        virtual void setStencilFunc(SR_COMPARISON_FUNC func) = 0;

        virtual void setStencilBufferReadOnly(bool readOnly) = 0;

        virtual void setStencilMask(uint32 mask) = 0;

        [[nodiscard]] virtual uint64 getIdentifier() = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] float getClearDepth() const;

        [[nodiscard]] uint32 getClearStencil() const;

        [[nodiscard]] bool isDepthTestEnabled() const;

        [[nodiscard]] bool isDepthBufferReadOnly() const;

        [[nodiscard]] SR_COMPARISON_FUNC getDepthFunc() const;

        [[nodiscard]] bool isStencilTestEnabled() const;

        [[nodiscard]] bool isStencilBufferReadOnly() const;

        [[nodiscard]] SR_COMPARISON_FUNC getStencilFunc() const;

        [[nodiscard]] uint32 getStencilMask() const;

    protected:
        uint32 m_Width;
        uint32 m_Height;
        float m_ClearDepth;
        uint32 m_ClearStencil;

        bool m_EnableDepthTest;
        bool m_DepthBufferReadOnly;
        SR_COMPARISON_FUNC m_DepthFunc;

        bool m_EnableStencilTest;
        bool m_StencilBufferReadOnly;
        SR_COMPARISON_FUNC m_StencilFunc;
        uint32 m_StencilMask;
    };
}
