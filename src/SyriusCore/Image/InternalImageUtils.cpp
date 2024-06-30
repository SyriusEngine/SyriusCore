#include "InternalImageUtils.hpp"

namespace Syrius{

    SR_TEXTURE_FORMAT getFormatFromChannelCount(uint32 channelCount, SR_DATA_TYPE dataType){
        // upper 4 bits is channel count, lower 4 bits is data type
        auto castedChannelCount = static_cast<uint8>(channelCount);
        auto shifted = (dataType & 0xF0) >> 4;
        return static_cast<SR_TEXTURE_FORMAT>((castedChannelCount << 4) | shifted);
    }

}
