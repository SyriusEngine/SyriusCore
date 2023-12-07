#include "../../../include/SyriusCore/Image/ImageFactory.hpp"

#include "ImageUI8.hpp"
#include "ImageF32.hpp"

#include "../Dependencies/stb_image/stb_image.hpp"
#include "../Dependencies/stb_image/stb_image_resize.hpp"
#include "../Dependencies/stb_image/stb_image_write.hpp"

#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Resource<Image> createImage(const ImageFileDesc& desc){
        if (stbi_is_hdr(desc.fileName.c_str())){
            auto img = new ImageF32(desc);
            return Resource<Image>(img);
        }
        else{
            auto img = new ImageUI8(desc);
            return Resource<Image>(img);
        }
    }

    Resource<Image> createImageF32(const ImageF32Desc& desc){
        auto img = new ImageF32(desc);
        return Resource<Image>(img);
    }

    Resource<Image> createImageUI8(const ImageUI8Desc& desc){
        auto img = new ImageUI8(desc);
        return Resource<Image>(img);
    }

}
