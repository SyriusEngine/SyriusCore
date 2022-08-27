#pragma once

#include "../Utils/Image.hpp"
#include "Utils.hpp"

namespace Syrius{

    class SR_API Texture2D{
    public:
        Texture2D();

        virtual ~Texture2D();

        virtual void bind(uint32_t slot) = 0;

        virtual void unbind() = 0;

        virtual void upload(Image* image) = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

    };

}

