#pragma once

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
        float maxLOD = 0.0f;
        float LODBias = 0.0f;
    };

    class Sampler{
    public:
        explicit Sampler(const SamplerDesc& desc);

        virtual ~Sampler();

        virtual void bindShaderResource(uint32 slot) = 0;

        [[nodiscard]] virtual uint64 getIdentifier() = 0;

    };

}
