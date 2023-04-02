#pragma once

#include "../Utils/Image.hpp"
#include "../Utils/Resource.hpp"
#include "Utils.hpp"
#include "Sampler.hpp"

namespace Syrius{

    struct SR_CORE_API Texture2DDesc{
        uint32 width          = 0;
        uint32 height         = 0;
        ResourceView<Sampler> sampler;
        SR_TEXTURE_DATA_FORMAT format = SR_TEXTURE_DATA_RGBA_UI8;
        const void* data            = nullptr;

    };

    struct SR_CORE_API Texture2DImageDesc{
        ResourceView<Image> image;
        ResourceView<Sampler> sampler;
    };

    class SR_CORE_API Texture2D{
    public:
        explicit Texture2D(const Texture2DDesc& desc);

        explicit Texture2D(const Texture2DImageDesc& desc);

        virtual ~Texture2D();

        virtual void bind(uint32_t slot) = 0;

        virtual void unbind() = 0;

        virtual void setData(const void* data, uint32 x, uint32 y, uint32 width, uint32 height) = 0;

        inline void setData(Resource<Image>& img, uint32 x, uint32 y, uint32 width, uint32 height){
            auto* data = img->getData();
            setData(data, x, y, width, height);
        }

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_DATA_FORMAT getFormat() const;

    protected:
        uint32 m_Width;
        uint32 m_Height;
        SR_TEXTURE_DATA_FORMAT m_Format;

    };

}

