#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct SR_CORE_API DepthStencilAttachmentDesc{
        uint32 width                    = SR_DEFAULT_WIDTH;
        uint32 height                   = SR_DEFAULT_HEIGHT;
        SR_TEXTURE_DATA_FORMAT format   = SR_TEXTURE_DATA_DEPTH_24_STENCIL_8;

        bool enableShaderRead = true; // enables sampling from this attachment in shaders
        bool enableAutoResize           = true; // whenever the window is resized, this attachment will be resized as well

        bool enableDepthTest            = false;
        SR_COMPARISON_FUNC depthFunc    = SR_COMPARISON_FUNC_LESS;
        float clearDepth                = 1.0f;

        bool enableStencilTest                  = false;
        SR_COMPARISON_FUNC stencilFunc          = SR_COMPARISON_FUNC_ALWAYS;
        uint32 clearStencil                     = 0;
        uint32 stencilMask                      = 0xFFFFFFFF;
        SR_STENCIL_FUNC stencilFail             = SR_STENCIL_FUNC_KEEP;
        SR_STENCIL_FUNC stencilPass             = SR_STENCIL_FUNC_KEEP;
        SR_STENCIL_FUNC stencilPassDepthFail    = SR_STENCIL_FUNC_KEEP;
    };

    class SR_CORE_API DepthStencilAttachment{
    public:
        explicit DepthStencilAttachment(const DepthStencilAttachmentDesc& desc);

        virtual ~DepthStencilAttachment();

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual void bindShaderResource(uint32 slot) = 0;

        virtual void clear() = 0;

        virtual void setSize(uint32 width, uint32 height) = 0;

        [[nodiscard]] virtual Resource<Image> getData() = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_DATA_FORMAT getFormat() const;

        [[nodiscard]] bool shaderReadEnabled() const;

        [[nodiscard]] bool depthTestEnabled() const;

        [[nodiscard]] SR_COMPARISON_FUNC getDepthFunc() const;

        [[nodiscard]] float getClearDepth() const;

        [[nodiscard]] bool stencilTestEnabled() const;

        [[nodiscard]] SR_COMPARISON_FUNC getStencilFunc() const;

        [[nodiscard]] uint32 getClearStencil() const;

        [[nodiscard]] uint32 getStencilMask() const;

        [[nodiscard]] SR_STENCIL_FUNC getStencilFail() const;

        [[nodiscard]] SR_STENCIL_FUNC getStencilPass() const;

        [[nodiscard]] SR_STENCIL_FUNC getStencilPassDepthFail() const;

    protected:
        uint32 m_Width;
        uint32 m_Height;
        SR_TEXTURE_DATA_FORMAT m_Format;
        bool m_EnableShaderRead;

        bool m_EnableDepthTest;
        SR_COMPARISON_FUNC m_DepthFunc;
        float m_ClearDepth;

        bool m_EnableStencilTest;
        SR_COMPARISON_FUNC m_StencilFunc;
        uint32 m_ClearStencil;
        uint32 m_StencilMask;
        SR_STENCIL_FUNC m_StencilFail;
        SR_STENCIL_FUNC m_StencilPass;
        SR_STENCIL_FUNC m_StencilPassDepthFail;

    private:

        friend class FrameBuffer;

        void onResize(uint32 width, uint32 height); // called by the framebuffer when its owning window is resized

    private:
        bool m_EnableAutoResize;
    };

}