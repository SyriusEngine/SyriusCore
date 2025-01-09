#pragma once

#include "ImageUtils.hpp"

namespace Syrius{

    class SR_CORE_API Image{
    public:
        virtual ~Image();

        virtual void writeToFile(const ImageFileDesc& writeDesc) const = 0;

        virtual void resize(u32 width, u32 height) = 0;

        virtual void extendAlpha() = 0;

        [[nodiscard]] virtual void* getData() = 0;

        [[nodiscard]] virtual UP<Image> convertToUI8() = 0;

        [[nodiscard]] u32 getWidth() const;

        [[nodiscard]] u32 getHeight() const;

        [[nodiscard]] u32 getChannelCount() const;

        [[nodiscard]] SR_TEXTURE_FORMAT getFormat() const;

    protected:
        Image();

        Image(u32 width, u32 height, SR_TEXTURE_FORMAT format);

    protected:
        u32 m_Width;
        u32 m_Height;
        SR_TEXTURE_FORMAT m_Format;


    };

}