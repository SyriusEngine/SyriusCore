#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/Sampler2D.hpp"

namespace Syrius{

    class GlSampler2D: public Sampler2D{
    public:
        explicit GlSampler2D(const Sampler2DDesc& desc);

        ~GlSampler2D() override;

        void bind(uint32 slot) override;

        [[nodiscard]] uint64 getIdentifier() override;

    private:
        uint32 m_SamplerID;
    };

}
