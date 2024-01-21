#pragma once

#include "../../../../include/SyriusCore/Context/Texture2D.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11Texture2D: public Texture2D{
    public:
        D3D11Texture2D(const Texture2DDesc& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        D3D11Texture2D(const Texture2DImageDesc& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11Texture2D() override;

        void bind(uint32_t slot) override;

        void unbind() override;

        void setData(const void* data, uint32 x, uint32 y, uint32 width, uint32 height) override;

        Resource<Image> getData() override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:

        void createResources(const void* data);

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11Texture2D* m_Texture;
        ID3D11ShaderResourceView* m_TextureView;
    };

}

#endif

