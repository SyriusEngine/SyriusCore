#pragma once

#include "../../../include/SyriusCore/Image/ImageUtils.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    template<typename T>
    void addAlpha(const std::vector<T>& src, std::vector<T>& dst, uint32 width, uint32 height, uint32 channelCount){
        SR_CORE_PRECONDITION(channelCount == 3, "Invalid channel count: %i", channelCount);

        size_t numPixels = src.size() / 3;

        // Create a vector to store RGBA data
        dst.reserve(numPixels * 4);

        // Loop through each pixel
        for (size_t i = 0; i < numPixels; ++i) {
            // Copy RGB channels
            dst.push_back(src[i * 3]);     // Red
            dst.push_back(src[i * 3 + 1]); // Green
            dst.push_back(src[i * 3 + 2]); // Blue
            // Set alpha channel to 255 (fully opaque)
            dst.push_back(255);
        }
    }

    uint32 getBytesPerPixel(SR_TEXTURE_FORMAT format);
}

