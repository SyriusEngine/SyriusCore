#pragma once

#include "../Utils/Image.hpp"
#include "Utils.hpp"
#include "Sampler.hpp"

namespace Syrius{

    struct SR_API Texture2DDesc{
        Image* m_Image                  = nullptr;
        Sampler* m_Sampler2D          = nullptr;
        SR_TEXTURE_FILTER m_MinFilter   = SR_TEXTURE_FILTER_LINEAR;
        SR_TEXTURE_FILTER m_MagFilter   = SR_TEXTURE_FILTER_LINEAR;
        SR_TEXTURE_WRAP m_WrapAddressU  = SR_TEXTURE_WRAP_REPEAT;
        SR_TEXTURE_WRAP m_WrapAddressV  = SR_TEXTURE_WRAP_REPEAT;

    };

    class SR_API Texture2D{
    public:
        explicit Texture2D(const Texture2DDesc& desc);

        virtual ~Texture2D();

        virtual void bind(uint32_t slot) = 0;

        virtual void unbind() = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

    };

}

