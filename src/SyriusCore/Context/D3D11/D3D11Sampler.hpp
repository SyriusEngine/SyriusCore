#pragma once

#include "../../../../include/SyriusCore/Context/Sampler.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11Sampler: public Sampler{
    public:
        D3D11Sampler(const SamplerDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11Sampler() override;

        void bindShaderResource(uint32 slot) override;

        [[nodiscard]] uint64 getIdentifier() override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11SamplerState* m_Sampler;
    };

}

#endif
