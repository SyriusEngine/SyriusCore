#include "../../../include/SyriusCore/Image/Image.hpp"

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
        auto formatType = getTextureDataType(desc.format);
        switch (formatType) {
            case SR_UINT8:{
                ImageUI8Desc imgDesc;
                imgDesc.data = reinterpret_cast<const ubyte*>(desc.data);
                imgDesc.width = desc.width;
                imgDesc.height = desc.height;
                imgDesc.format = desc.format;
                return Resource<Image>(new ImageUI8(imgDesc));
            }
            case SR_UINT16:{
                ImageUI16Desc imgDesc;
                imgDesc.data = reinterpret_cast<const uint16*>(desc.data);
                imgDesc.width = desc.width;
                imgDesc.height = desc.height;
                imgDesc.format = desc.format;
                return Resource<Image>(new ImageUI16(imgDesc));
            }
            case SR_FLOAT32:{
                ImageF32Desc imgDesc;
                imgDesc.data = reinterpret_cast<const float*>(desc.data);
                imgDesc.width = desc.width;
                imgDesc.height = desc.height;
                imgDesc.format = desc.format;
                return Resource<Image>(new ImageF32(imgDesc));
            }
            default:{
            }

        }
    }

    Resource<Image> createImage(const ImageF32Desc& desc){
        auto img = new ImageF32(desc);
        return Resource<Image>(img);
    }

    Resource<Image> createImage(const ImageUI16Desc& desc){
        auto img = new ImageUI16(desc);
        return Resource<Image>(img);
    }

    Resource<Image> createImage(const ImageUI8Desc& desc){
        auto img = new ImageUI8(desc);
        return Resource<Image>(img);
    }

}
