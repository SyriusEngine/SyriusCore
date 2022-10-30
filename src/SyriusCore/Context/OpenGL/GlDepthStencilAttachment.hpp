#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/DepthStencilAttachment.hpp"

namespace Syrius{

    class GlDepthStencilAttachment: public DepthStencilAttachment{
    public:
        explicit GlDepthStencilAttachment(const DepthStencilAttachmentDesc& desc);

        ~GlDepthStencilAttachment() override;

        void bind() override;

        void bindAsTexture(uint32 slot) override;

        void clear() override;

        void onResize(uint32 width, uint32 height) override;

        void setDepthFunc(SR_COMPARISON_FUNC func) override;

        void setDepthBufferReadOnly(bool readOnly) override;

        void setStencilFunc(SR_COMPARISON_FUNC func) override;

        void setStencilBufferReadOnly(bool readOnly) override;

        void setStencilMask(uint32 mask) override;

        [[nodiscard]] uint64 getIdentifier() override;

    private:
        uint32 m_BufferID;
    };

    class GlDefaultDepthStencilAttachment: public DepthStencilAttachment{
    public:
        explicit GlDefaultDepthStencilAttachment(const DepthStencilAttachmentDesc& desc);

        ~GlDefaultDepthStencilAttachment() override;

        void bind() override;

        void bindAsTexture(uint32 slot) override;

        void clear() override;

        void onResize(uint32 width, uint32 height) override;

        void setDepthFunc(SR_COMPARISON_FUNC func) override;

        void setDepthBufferReadOnly(bool readOnly) override;

        void setStencilFunc(SR_COMPARISON_FUNC func) override;

        void setStencilBufferReadOnly(bool readOnly) override;

        void setStencilMask(uint32 mask) override;

        [[nodiscard]] uint64 getIdentifier() override;

    private:
        GLenum m_GlDepthFunc;

        GLenum m_GlStencilFunc;
        GLenum m_GlStencilFail;
        GLenum m_GlStencilPassDepthFail;
        GLenum m_GlStencilPass;
    };

}