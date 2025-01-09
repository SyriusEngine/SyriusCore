#pragma once

#include "Utils.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API ColorAttachmentDesc{
        u32 width                = SR_DEFAULT_WIDTH;
        u32 height               = SR_DEFAULT_HEIGHT;
        float clearColor[4]         = {0.1f, 0.2f, 0.3f, 1.0f};
        SR_TEXTURE_FORMAT format    = SR_TEXTURE_RGBA_F32;
    };

    class SR_CORE_API ColorAttachment{
    public:
        ColorAttachment(const ColorAttachmentDesc& desc, const UP<DeviceLimits>& deviceLimits);

        virtual ~ColorAttachment();

        void setClearColor(float r, float g, float b, float a);

        virtual void bind() = 0;

        virtual void bindShaderResource(u32 slot) = 0;

        virtual void clear() = 0;

        virtual void onResize(u32 width, u32 height) = 0;

        [[nodiscard]] virtual UP<Image> getData() = 0;

        [[nodiscard]] virtual u64 getIdentifier() const = 0;

        [[nodiscard]] u32 getWidth() const;

        [[nodiscard]] u32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_FORMAT getFormat() const;

        [[nodiscard]] float* getClearColor();

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;

        u32 m_Width;
        u32 m_Height;
        float m_ClearColor[4];
        SR_TEXTURE_FORMAT m_Format;
    };

}