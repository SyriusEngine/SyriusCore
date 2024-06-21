#include "InternalImageUtils.hpp"

namespace Syrius{

    uint32 getBytesPerPixel(SR_TEXTURE_FORMAT format){
        auto channelCount = getTextureChannelCount(format);
        auto dataType = getTextureDataType(format);
        return channelCount * getTypeSize(dataType);
    }

}
