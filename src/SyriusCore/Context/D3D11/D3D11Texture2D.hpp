#pragma once

#include "../../../../include/SyriusCore/Context/Texture2D.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11Texture2D: public Texture2D{
    public:
        D3D11Texture2D(const Texture2DDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        D3D11Texture2D(const Texture2DImageDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11Texture2D() override;

        void bind() override;

        void bindShaderResource(u32 slot) override;

        void setData(const void* data, u32 x, u32 y, u32 width, u32 height) override;

        void copyFrom(const ResourceView<Texture2D>& other) override;

        void copyFrom(const ResourceView<ColorAttachment>& other) override;

        UP<Image> getData() override;

        [[nodiscard]] u64 getIdentifier() const override;

        [[nodiscard]] inline ID3D11Texture2D* getTexture() const { return m_Texture; }

        [[nodiscard]] inline ID3D11ShaderResourceView* getTextureView() const { return m_TextureView; }

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

