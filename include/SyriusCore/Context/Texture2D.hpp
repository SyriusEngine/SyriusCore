#pragma once

#include "../Utils/Image.hpp"
#include "Utils.hpp"
#include "Sampler.hpp"

namespace Syrius{

    struct SR_CORE_API Texture2DDesc{
        Image* m_Image          = nullptr;

    };

    class SR_CORE_API Texture2D{
    public:
        explicit Texture2D(const Texture2DDesc& desc);

        virtual ~Texture2D();

        virtual void bind(uint32_t slot) = 0;

        virtual void unbind() = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

    };

}

