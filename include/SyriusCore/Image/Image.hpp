#pragma once

#include "ImageBase.hpp"
#include "../Utils/Resource.hpp"

namespace Syrius{

    SR_CORE_API Resource<Image> createImage(const ImageFileDesc& desc);

    SR_CORE_API Resource<Image> createImage(const ImageF32Desc& desc);

    SR_CORE_API Resource<Image> createImage(const ImageUI16Desc& desc);

    SR_CORE_API Resource<Image> createImage(const ImageUI8Desc& desc);

}