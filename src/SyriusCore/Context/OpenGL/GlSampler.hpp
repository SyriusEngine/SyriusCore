#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/Sampler.hpp"

namespace Syrius{

    class GlSampler: public Sampler{
    public:
        explicit GlSampler(const SamplerDesc& desc);

        ~GlSampler() override;

        void bindShaderResource(uint32 slot) override;

        [[nodiscard]] uint64 getIdentifier() override;

    private:
        uint32 m_SamplerID;
    };

}
