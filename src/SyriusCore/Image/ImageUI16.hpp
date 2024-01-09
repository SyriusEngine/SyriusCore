#pragma once

#include "../../../include/SyriusCore/Image/Image.hpp"
#include "InternalUtils.hpp"

namespace Syrius{

    class ImageUI16 : public Image{
    public:
        explicit ImageUI16(const ImageFileDesc& desc);

        explicit ImageUI16(const ImageUI16Desc& desc);

        ~ImageUI16() override;

        void writeToFile(const ImageFileDesc& writeDesc) const override;

        void resize(uint32 width, uint32 height) override;

        void extendAlpha() override;

        [[nodiscard]] const void* getData() const override;

    private:
        std::vector<uint16> m_Data;
    };
}