#pragma once

#include <cfloat>
#include "Utils.hpp"

namespace Syrius{

    struct SamplerDesc{
        SR_TEXTURE_FILTER minFilter   = SR_TEXTURE_FILTER_LINEAR;
        SR_TEXTURE_FILTER magFilter   = SR_TEXTURE_FILTER_LINEAR;
        SR_TEXTURE_WRAP wrapU  = SR_TEXTURE_WRAP_REPEAT;
        SR_TEXTURE_WRAP wrapV  = SR_TEXTURE_WRAP_REPEAT;
        SR_TEXTURE_WRAP wrapW  = SR_TEXTURE_WRAP_REPEAT;
        SR_COMPARISON_FUNC comparisonFunc = SR_COMPARISON_FUNC_NEVER;
        float borderColor[4] = {0.0f, 0.0f, 0.0f, 0.0f };
        float minLOD = 0.0f;
        float maxLOD = FLT_MAX;
        float LODBias = 0.0f;
    };

    class SR_CORE_API Sampler{
    public:
        explicit Sampler(const SamplerDesc& desc);

        virtual ~Sampler();

        virtual void bindShaderResource(u32 slot) = 0;

        virtual void setFilter(SR_TEXTURE_FILTER minFilter, SR_TEXTURE_FILTER magFilter) = 0;

        virtual void setWrap(SR_TEXTURE_WRAP wrapU, SR_TEXTURE_WRAP wrapV, SR_TEXTURE_WRAP wrapW) = 0;

        virtual void setComparisonFunc(SR_COMPARISON_FUNC comparisonFunc) = 0;

        virtual void setBorderColor(const float* borderColor) = 0;

        virtual void setLOD(float minLOD, float maxLOD, float LODBias) = 0;

        [[nodiscard]] virtual u64 getIdentifier() = 0;

        [[nodiscard]] SR_TEXTURE_FILTER getMinFilter() const;

        [[nodiscard]] SR_TEXTURE_FILTER getMagFilter() const;

        [[nodiscard]] SR_TEXTURE_WRAP getWrapU() const;

        [[nodiscard]] SR_TEXTURE_WRAP getWrapV() const;

        [[nodiscard]] SR_TEXTURE_WRAP getWrapW() const;

        [[nodiscard]] SR_COMPARISON_FUNC getComparisonFunc() const;

        [[nodiscard]] const float* getBorderColor() const;

        [[nodiscard]] float getMinLOD() const;

        [[nodiscard]] float getMaxLOD() const;

        [[nodiscard]] float getLODBias() const;

    protected:
        SR_TEXTURE_FILTER m_MinFilter;
        SR_TEXTURE_FILTER m_MagFilter;
        SR_TEXTURE_WRAP m_WrapU;
        SR_TEXTURE_WRAP m_WrapV;
        SR_TEXTURE_WRAP m_WrapW;
        SR_COMPARISON_FUNC m_ComparisonFunc;
        float m_BorderColor[4];
        float m_MinLOD;
        float m_MaxLOD;
        float m_LODBias;

    };

}
