#pragma once

#include "../../../include/SyriusCore/Image/ImageBase.hpp"
#include "InternalImageUtils.hpp"

namespace Syrius{

    class ImageUI16 : public Image{
    public:
        explicit ImageUI16(const ImageFileDesc& desc);

        explicit ImageUI16(const ImageUI16Desc& desc);

        ~ImageUI16() override;

        void writeToFile(const ImageFileDesc& writeDesc) const override;

        void resize(u32 width, u32 height) override;

        void extendAlpha() override;

        [[nodiscard]] void* getData() override;

        [[nodiscard]] UP<Image> convertToUI8() override;

    private:
        std::vector<u16> m_Data;
    };
}