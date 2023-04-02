#pragma once

#include "ImageData.hpp"

namespace Syrius{

    class ImageDataF32 : public ImageData{
    public:
        ImageDataF32(const std::string& fileName, bool flipOnLoad);

        ImageDataF32(const float* pixelData, int32 width, int32 height, SR_TEXTURE_FORMAT format);

        ~ImageDataF32() override;

        void writeToFile(const std::string& fileName, bool flipOnWrite, SR_IMAGE_TYPE imgType) const override;

        void resize(int32 newWidth, int32 newHeight) override;

        [[nodiscard]] const void* getData() const override;

        void extendAlpha(ubyte alpha) override;

    private:
        std::vector<float> m_Data;
    };

}