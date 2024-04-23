#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/Sampler.hpp"

namespace Syrius{

    class GlSampler: public Sampler{
    public:
        explicit GlSampler(const SamplerDesc& desc);

        ~GlSampler() override;

        void bindShaderResource(uint32 slot) override;

        void setFilter(SR_TEXTURE_FILTER minFilter, SR_TEXTURE_FILTER magFilter) override;

        void setWrap(SR_TEXTURE_WRAP wrapU, SR_TEXTURE_WRAP wrapV, SR_TEXTURE_WRAP wrapW) override;

        void setComparisonFunc(SR_COMPARISON_FUNC comparisonFunc) override;

        void setBorderColor(const float* borderColor) override;

        void setLOD(float minLOD, float maxLOD, float LODBias) override;

        [[nodiscard]] uint64 getIdentifier() override;

    private:
        uint32 m_SamplerID;
    };

}
