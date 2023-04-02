#pragma once

#include "../../../include/SyriusCore/Core/SyriusCoreInclude.hpp"
#include "../../../include/SyriusCore/Utils/Image.hpp"

#include "../Dependencies/stb_image/stb_image.hpp"
#include "../Dependencies/stb_image/stb_image_resize.hpp"
#include "../Dependencies/stb_image/stb_image_write.hpp"

#include "../Core/DebugMacros.hpp"

namespace Syrius{

    class ImageData{
    public:
        virtual ~ImageData() = default;

        virtual void writeToFile(const std::string& fileName, bool flipOnWrite, SR_IMAGE_TYPE imgType) const = 0;

        virtual void resize(int32 newWidth, int32 newHeight) = 0;

        [[nodiscard]] virtual const void* getData() const = 0;

        virtual void extendAlpha(ubyte alpha) = 0;

    protected:

        ImageData(int32 width, int32 height, int32 channelCount, SR_TEXTURE_FORMAT format):
        m_Width(width),
        m_Height(height),
        m_ChannelCount(channelCount),
        m_Format(format){

        }

    protected:

        friend class Image;

        int32 m_Width;
        int32 m_Height;
        int32 m_ChannelCount;

        SR_TEXTURE_FORMAT m_Format;
    };

    template<typename T>
    void extendAlpha(std::vector<T>& data, int32 width, int32 height, int32 channelCount, T alpha) {
        SR_CORE_PRECONDITION(channelCount == 3, "Channel count must be 3")
        std::vector<T> tempData = data;
        data.resize(width * height * 4);

        for (int32 y = 0; y < height; y++){
            for (int32 x = 0; x < width; x++){
                for (int32 c = 0; c < channelCount; c++){
                    data[y * width * 4 + x * 4 + c] = tempData[y * width * 3 + x * 3 + c];
                }
                data[y * width * 4 + x * 4 + 3] = alpha;
            }
        }
    }
}