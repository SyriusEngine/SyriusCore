#pragma once

#include "../../../../include/SyriusCore/Context/CubeMap.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11CubeMap : public CubeMap{
    public:
        D3D11CubeMap(const ResourceView<CubeMapLayout>& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11CubeMap() override;

        void bind() override;

        void bindShaderResource(uint32 slot) override;

        void copyFrom(const ResourceView<Texture2D>& other, SR_CUBEMAP_FACE destinationFace) override;

        void copyFrom(const ResourceView<ColorAttachment>& other, SR_CUBEMAP_FACE destinationFace) override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11Texture2D* m_Texture;
        ID3D11ShaderResourceView* m_TextureView;
    };

}

#endif