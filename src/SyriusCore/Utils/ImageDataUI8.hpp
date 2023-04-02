#pragma once

#include "ImageData.hpp"

namespace Syrius{

    class ImageDataUI8 : public ImageData{
    public:
        ImageDataUI8(const std::string& fileName, bool flipOnLoad);

        ImageDataUI8(const ubyte* pixelData, int32 width, int32 height, SR_TEXTURE_FORMAT format);

        ~ImageDataUI8() override;

        void writeToFile(const std::string& fileName, bool flipOnWrite, SR_IMAGE_TYPE imgType) const override;

        void resize(int32 newWidth, int32 newHeight) override;

        [[nodiscard]] const void* getData() const override;

        void extendAlpha(ubyte alpha) override;

    private:
        std::vector<ubyte> m_Data;
    };

}