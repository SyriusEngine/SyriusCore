#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct Sampler2DDesc{
        SR_TEXTURE_FILTER m_MinFilter   = SR_TEXTURE_FILTER_LINEAR;
        SR_TEXTURE_FILTER m_MagFilter   = SR_TEXTURE_FILTER_LINEAR;
        SR_TEXTURE_WRAP m_WrapU  = SR_TEXTURE_WRAP_REPEAT;
        SR_TEXTURE_WRAP m_WrapV  = SR_TEXTURE_WRAP_REPEAT;
        SR_TEXTURE_WRAP m_WrapW  = SR_TEXTURE_WRAP_REPEAT;
        SR_COMPARISON_FUNC m_ComparisonFunc = SR_COMPARISON_FUNC_NEVER;
        float m_BorderColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        float m_MinLOD = 0.0f;
        float m_MaxLOD = 0.0f;
        float m_LODBias = 0.0f;
    };

    class Sampler{
    public:
        explicit Sampler(const Sampler2DDesc& desc);

        virtual ~Sampler();

        virtual void bind(uint32 slot) = 0;

        [[nodiscard]] virtual uint64 getIdentifier() = 0;

        [[nodiscard]] const Sampler2DDesc& getDesc() const;

    protected:
        Sampler2DDesc m_Desc;
    };

}
