#include "../../../include/SyriusCore/Image/ImageFactory.hpp"

#include "ImageImpl.hpp"
#include "ImageUI8.hpp"
#include "ImageUI16.hpp"
#include "ImageF32.hpp"

#include "../Dependencies/stb_image/stb_image.hpp"
#include "../Dependencies/stb_image/stb_image_resize.hpp"
#include "../Dependencies/stb_image/stb_image_write.hpp"

#include "../Utils/DebugMacros.hpp"

#include <filesystem>

namespace Syrius{

    Resource<Image> createImage(const ImageFileDesc& desc){
        SR_CORE_ASSERT(std::filesystem::exists(desc.fileName), "File: %s does not exist!", desc.fileName.c_str());

        if (stbi_is_hdr(desc.fileName.c_str())){
            auto img = new ImageF32(desc);
            return Resource<Image>(img);
        }
        else if (stbi_is_16_bit(desc.fileName.c_str())){
            auto img = new ImageUI16(desc);
            return Resource<Image>(img);
        }
        else{
            auto img = new ImageUI8(desc);
            return Resource<Image>(img);
        }
    }

    Resource<Image> createImage(const ImageDesc& desc){
        auto img = new ImageImpl(desc);
        return Resource<Image>(img);
    }

    Resource<Image> createImage(const ImageF32Desc& desc){
        auto img = new ImageF32(desc);
        return Resource<Image>(img);
    }

    Resource<Image> createImage(const ImageUI8Desc& desc){
        auto img = new ImageUI8(desc);
        return Resource<Image>(img);
    }

}
