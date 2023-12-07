#pragma once

#include "../../../include/SyriusCore/Image/Image.hpp"

namespace Syrius{

    class ImageUI8 : public Image{
    public:
        explicit ImageUI8(const ImageFileDesc& desc);

        explicit ImageUI8(const ImageUI8Desc& desc);

        ~ImageUI8() override;

        void writeToFile(const ImageFileDesc& writeDesc) const override;

        void resize(uint32 width, uint32 height) override;

        void extendAlpha() override;

        [[nodiscard]] const void* getData() const override;

    private:
        std::vector<ubyte> m_Data;

    };

}