#pragma once

#include "../../../include/SyriusCore/Image/ImageBase.hpp"
#include "InternalImageUtils.hpp"

namespace Syrius{

    class ImageT: public Image{
    public:
        explicit ImageT(const ImageDesc& desc);

        ~ImageT() override;

        void writeToFile(const ImageFileDesc& writeDesc) const override;

        void resize(uint32 width, uint32 height) override;

        void extendAlpha() override;

        [[nodiscard]] void* getData() override;

        [[nodiscard]] Resource<Image> convertToUI8() override;

    private:
        Resource<UByte> m_Data;
    };
}