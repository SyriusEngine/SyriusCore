#pragma once

#include "../../../include/SyriusCore/Image/ImageBase.hpp"
#include "InternalImageUtils.hpp"

namespace Syrius{

    class ImageUI8 : public Image{
    public:
        explicit ImageUI8(const ImageFileDesc& desc);

        explicit ImageUI8(const ImageUI8Desc& desc);

        ~ImageUI8() override;

        void writeToFile(const ImageFileDesc& writeDesc) const override;

        void resize(u32 width, u32 height) override;

        void extendAlpha() override;

        [[nodiscard]] void* getData() override;

        [[nodiscard]] UP<Image> convertToUI8() override;

    private:
        std::vector<UByte> m_Data;

    };

}