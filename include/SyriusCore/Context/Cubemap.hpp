#pragma once

#include "../Image/Image.hpp"
#include "../Utils/Resource.hpp"
#include "Utils.hpp"

namespace Syrius{

    struct SR_CORE_API CubemapFaceDesc{
        uint32 width          = 0;
        uint32 height         = 0;
        SR_TEXTURE_FORMAT format = SR_TEXTURE_RGBA_UI8;
        const void* data            = nullptr;
    };

    struct SR_CORE_API CubemapDesc{
        uint32 width          = 0;
        uint32 height         = 0;
        SR_TEXTURE_FORMAT format = SR_TEXTURE_RGBA_UI8;
                                        // right,   left,    top,     bottom,  front,   back
        const void* faces[6]            = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    };

    struct SR_CORE_API CubemapImageDesc{
        ResourceView<Image> faces[6];
    };

    class SR_CORE_API Cubemap{
    public:
        explicit Cubemap(const CubemapDesc& desc);

        explicit Cubemap(const CubemapImageDesc& desc);

        virtual ~Cubemap();

        virtual void bind(uint32 slot) = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_FORMAT getFormat() const;

    protected:
        const uint32 m_Width;
        const uint32 m_Height;
        const SR_TEXTURE_FORMAT m_Format;


    };

}