#pragma once

#include "ImageBase.hpp"
#include <SyriusUtils/SyriusUtils.hpp>

namespace Syrius{

    SR_CORE_API UP<Image> createImage(const ImageFileDesc& desc);

    SR_CORE_API UP<Image> createImage(const ImageDesc& desc);

    SR_CORE_API UP<Image> createImage(const ImageF32Desc& desc);

    SR_CORE_API UP<Image> createImage(const ImageUI16Desc& desc);

    SR_CORE_API UP<Image> createImage(const ImageUI8Desc& desc);

}