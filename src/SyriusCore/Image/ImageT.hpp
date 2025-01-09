#pragma once

#include "../../../include/SyriusCore/Image/ImageBase.hpp"
#include "InternalImageUtils.hpp"

namespace Syrius{

    class ImageT: public Image{
    public:
        explicit ImageT(const ImageDesc& desc);

        ~ImageT() override;

        void writeToFile(const ImageFileDesc& writeDesc) const override;

        void resize(u32 width, u32 height) override;

        void extendAlpha() override;

        [[nodiscard]] void* getData() override;

        [[nodiscard]] UP<Image> convertToUI8() override;

    private:
        UP<UByte> m_Data;
    };
}