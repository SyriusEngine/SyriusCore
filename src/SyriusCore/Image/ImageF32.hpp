#pragma once

#include "../../../include/SyriusCore/Image/Image.hpp"

namespace Syrius{

    class ImageF32 : public Image{
    public:
        explicit ImageF32(const ImageFileDesc& desc);

        explicit ImageF32(const ImageF32Desc& desc);

        ~ImageF32() override;

        void writeToFile(const ImageFileDesc& desc) const override;

        void resize(uint32 width, uint32 height) override;

        void extendAlpha() override;

        [[nodiscard]] const void* getData() const override;

    private:
        std::vector<float> m_Data;

    };

}