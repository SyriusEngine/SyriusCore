#pragma once

#include "ImageUtils.hpp"

namespace Syrius{

    class SR_CORE_API Image{
    public:
        virtual ~Image();

        virtual void writeToFile(const ImageFileDesc& writeDesc) const = 0;

        virtual void resize(uint32 width, uint32 height) = 0;

        virtual void extendAlpha() = 0;

        [[nodiscard]] virtual void* getData() = 0;

        [[nodiscard]] virtual Resource<Image> convertToUI8() = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] uint32 getChannelCount() const;

        [[nodiscard]] SR_TEXTURE_FORMAT getFormat() const;

    protected:
        Image();

        Image(uint32 width, uint32 height, SR_TEXTURE_FORMAT format);

    protected:
        uint32 m_Width;
        uint32 m_Height;
        SR_TEXTURE_FORMAT m_Format;


    };

}