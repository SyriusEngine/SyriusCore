#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct Sampler2DDesc{
        SR_TEXTURE_FILTER m_MinFilter   = SR_TEXTURE_FILTER_LINEAR;
        SR_TEXTURE_FILTER m_MagFilter   = SR_TEXTURE_FILTER_LINEAR;
        SR_TEXTURE_WRAP m_WrapU  = SR_TEXTURE_WRAP_REPEAT;
        SR_TEXTURE_WRAP m_WrapV  = SR_TEXTURE_WRAP_REPEAT;
    };

    class Sampler2D{
    public:
        explicit Sampler2D(const Sampler2DDesc& desc);

        virtual ~Sampler2D();

        virtual void bind(uint32 slot) = 0;

        [[nodiscard]] virtual uint64 getIdentifier() = 0;

        [[nodiscard]] const Sampler2DDesc& getDesc() const;

    protected:
        Sampler2DDesc m_Desc;
    };

}
