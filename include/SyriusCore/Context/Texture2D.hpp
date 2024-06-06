#pragma once

#include "../Image/ImageBase.hpp"
#include "../Utils/Resource.hpp"
#include "Utils.hpp"
#include "Sampler.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API Texture2DDesc{
        uint32 width                = 0;
        uint32 height               = 0;
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
        Texture2D(const Texture2DDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        Texture2D(const Texture2DImageDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        virtual ~Texture2D();

        virtual void bind() = 0;

        virtual void bindShaderResource(uint32 slot) = 0;

        virtual void setData(const void* data, uint32 x, uint32 y, uint32 width, uint32 height) = 0;

        virtual Resource<Image> getData() = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_FORMAT getFormat() const;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const;

    protected:
        const Resource<DeviceLimits>& m_DeviceLimits;

        uint32 m_Width;
        uint32 m_Height;
        SR_TEXTURE_FORMAT m_Format;
        SR_BUFFER_USAGE m_Usage;

    };

}

