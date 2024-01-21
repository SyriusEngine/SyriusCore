#pragma once

#include "../../../../include/SyriusCore/Context/Cubemap.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11Cubemap : public Cubemap{
    public:
        D3D11Cubemap(const CubemapDesc& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        D3D11Cubemap(const CubemapImageDesc& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11Cubemap() override;

        void bind(uint32 slot) override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:

        void createResources(const void* faces[6] );

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11Texture2D* m_Texture;
        ID3D11ShaderResourceView* m_TextureView;
    };

}

#endif