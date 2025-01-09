#pragma once

#include "../Image/ImageBase.hpp"
#include "Utils.hpp"
#include "Sampler.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    class ColorAttachment;

    struct SR_CORE_API Texture2DDesc{
        u32 width                = 0;
        u32 height               = 0;
        SR_TEXTURE_FORMAT format    = SR_TEXTURE_RGBA_UI8;
        const void* data            = nullptr;
        SR_BUFFER_USAGE usage       = SR_BUFFER_USAGE_DEFAULT;

    };

    struct SR_CORE_API Texture2DImageDesc{
        ResourceView<Image> image;
        SR_BUFFER_USAGE usage       = SR_BUFFER_USAGE_DEFAULT;
    };

    class SR_CORE_API Texture2D{
    public:
        Texture2D(const Texture2DDesc& desc, const UP<DeviceLimits>& deviceLimits);

        Texture2D(const Texture2DImageDesc& desc, const UP<DeviceLimits>& deviceLimits);

        virtual ~Texture2D();

        virtual void bind() = 0;

        virtual void bindShaderResource(u32 slot) = 0;

        virtual void setData(const void* data, u32 x, u32 y, u32 width, u32 height) = 0;

        virtual void copyFrom(const ResourceView<Texture2D>& other) = 0;

        virtual void copyFrom(const ResourceView<ColorAttachment>& other) = 0;

        virtual UP<Image> getData() = 0;

        [[nodiscard]] virtual u64 getIdentifier() const = 0;

        [[nodiscard]] u32 getWidth() const;

        [[nodiscard]] u32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_FORMAT getFormat() const;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const;

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;

        u32 m_Width;
        u32 m_Height;
        SR_TEXTURE_FORMAT m_Format;
        SR_BUFFER_USAGE m_Usage;

    };

}

