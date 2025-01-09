#pragma once

#include "Utils.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API DepthStencilAttachmentDesc{
        u32 width               = SR_DEFAULT_WIDTH;
        u32 height              = SR_DEFAULT_HEIGHT;
        SR_TEXTURE_FORMAT format   = SR_TEXTURE_DEPTH_24_STENCIL_8;

        bool enableShaderAccess = false; // enables sampling from this attachment in shaders

        bool enableDepthTest            = false;
        SR_COMPARISON_FUNC depthFunc    = SR_COMPARISON_FUNC_LESS;
        float clearDepth                = 1.0f;
        SR_DEPTH_MASK depthMask         = SR_DEPTH_MASK_ENABLED;

        bool enableStencilTest                  = false;
        SR_COMPARISON_FUNC stencilFunc          = SR_COMPARISON_FUNC_ALWAYS;
        u32 clearStencil                     = 0;
        u32 stencilMask                      = 0xFFFFFFFF;
        SR_STENCIL_FUNC stencilFail             = SR_STENCIL_FUNC_KEEP;
        SR_STENCIL_FUNC stencilPass             = SR_STENCIL_FUNC_KEEP;
        SR_STENCIL_FUNC stencilPassDepthFail    = SR_STENCIL_FUNC_KEEP;
    };

    class SR_CORE_API DepthStencilAttachment{
    public:
        DepthStencilAttachment(const DepthStencilAttachmentDesc& desc, const UP<DeviceLimits>& deviceLimits);

        virtual ~DepthStencilAttachment();

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual void bindShaderResource(u32 slot) = 0;

        virtual void clear() = 0;

        virtual void onResize(u32 width, u32 height) = 0;

        virtual void enableDepthTest(bool enable) = 0;

        virtual void setDepthFunc(SR_COMPARISON_FUNC func) = 0;

        virtual void setDepthMask(SR_DEPTH_MASK mask) = 0;

        void setClearDepth(float depth);

        [[nodiscard]] virtual UP<Image> getData() = 0;

        [[nodiscard]] virtual u64 getIdentifier() const = 0;

        [[nodiscard]] u32 getWidth() const;

        [[nodiscard]] u32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_FORMAT getFormat() const;

        [[nodiscard]] bool shaderReadEnabled() const;

        [[nodiscard]] bool depthTestEnabled() const;

        [[nodiscard]] SR_COMPARISON_FUNC getDepthFunc() const;

        [[nodiscard]] float getClearDepth() const;

        [[nodiscard]] bool stencilTestEnabled() const;

        [[nodiscard]] SR_COMPARISON_FUNC getStencilFunc() const;

        [[nodiscard]] u32 getClearStencil() const;

        [[nodiscard]] u32 getStencilMask() const;

        [[nodiscard]] SR_STENCIL_FUNC getStencilFail() const;

        [[nodiscard]] SR_STENCIL_FUNC getStencilPass() const;

        [[nodiscard]] SR_STENCIL_FUNC getStencilPassDepthFail() const;

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;

        u32 m_Width;
        u32 m_Height;
        SR_TEXTURE_FORMAT m_Format;
        const bool m_EnableShaderAccess;

        bool m_EnableDepthTest;
        SR_COMPARISON_FUNC m_DepthFunc;
        float m_ClearDepth;
        SR_DEPTH_MASK m_DepthMask;

        bool m_EnableStencilTest;
        SR_COMPARISON_FUNC m_StencilFunc;
        u32 m_ClearStencil;
        u32 m_StencilMask;
        SR_STENCIL_FUNC m_StencilFail;
        SR_STENCIL_FUNC m_StencilPass;
        SR_STENCIL_FUNC m_StencilPassDepthFail;
    };

}