#pragma once

#include "../../../../include/SyriusCore/Context/Sampler.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11Sampler: public Sampler{
    public:
        D3D11Sampler(const SamplerDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11Sampler() override;

        void bindShaderResource(u32 slot) override;

        void setFilter(SR_TEXTURE_FILTER minFilter, SR_TEXTURE_FILTER magFilter) override;

        void setWrap(SR_TEXTURE_WRAP wrapU, SR_TEXTURE_WRAP wrapV, SR_TEXTURE_WRAP wrapW) override;

        void setComparisonFunc(SR_COMPARISON_FUNC comparisonFunc) override;

        void setBorderColor(const float* borderColor) override;

        void setLOD(float minLOD, float maxLOD, float LODBias) override;

        [[nodiscard]] u64 getIdentifier() override;

    private:

        void createResources();

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11SamplerState* m_Sampler;
    };

}

#endif
