#pragma once

#include "../../../include/SyriusCore/Image/ImageBase.hpp"
#include "InternalImageUtils.hpp"

namespace Syrius{

    class ImageF32 : public Image{
    public:
        explicit ImageF32(const ImageFileDesc& desc);

        explicit ImageF32(const ImageF32Desc& desc);

        ~ImageF32() override;

        void writeToFile(const ImageFileDesc& desc) const override;

        void resize(u32 width, u32 height) override;

        void extendAlpha() override;

        [[nodiscard]] void* getData() override;

        [[nodiscard]] UP<Image> convertToUI8() override;

    private:
        std::vector<float> m_Data;

    };

}