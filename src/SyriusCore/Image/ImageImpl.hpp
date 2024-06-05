#pragma once

#include "../../../include/SyriusCore/Image/Image.hpp"

namespace Syrius{

    class ImageImpl : public Image{
    public:
        explicit ImageImpl(const ImageDesc& desc);

        ~ImageImpl() override;

        void writeToFile(const ImageFileDesc& writeDesc) const override;

        void resize(uint32 width, uint32 height) override;

        void extendAlpha() override;

        [[nodiscard]] void* getData() override;

    private:
        void* m_Data;
        uint64 m_Size;

    };

}