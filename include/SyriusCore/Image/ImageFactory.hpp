#pragma once

#include "Image.hpp"
#include "../Utils/Resource.hpp"

namespace Syrius{

    SR_CORE_API Resource<Image> createImage(const ImageFileDesc& desc);

    SR_CORE_API Resource<Image> createImage(const ImageDesc& desc);

    SR_CORE_API Resource<Image> createImageF32(const ImageF32Desc& desc);

    SR_CORE_API Resource<Image> createImageUI8(const ImageUI8Desc& desc);

}