#pragma once

#include "../../../../include/SyriusCore/Context/ColorAttachment.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11ColorAttachment : public ColorAttachment {
    public:
        D3D11ColorAttachment(const ColorAttachmentDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *deviceContext);

        ~D3D11ColorAttachment() override;

        void bind() override;

        void bindShaderResource(u32 slot) override;

        void clear() override;

        void onResize(u32 width, u32 height) override;

        [[nodiscard]] UP<Image> getData() override;

        [[nodiscard]] u64 getIdentifier() const override;

        [[nodiscard]] ID3D11RenderTargetView *getRenderTargetView() const;

        [[nodiscard]] inline ID3D11Texture2D* getTexture() const { return m_Texture; }

        [[nodiscard]] inline ID3D11ShaderResourceView* getTextureView() const { return m_TextureView; }

    private:

        void createResources();

        void destroyResources();

    private:
        ID3D11Device *m_Device;
        ID3D11DeviceContext *m_Context;

        ID3D11Texture2D *m_Texture;
        ID3D11RenderTargetView *m_RenderTargetView;
        ID3D11ShaderResourceView *m_TextureView;
    };

    class D3D11DefaultColorAttachment : public ColorAttachment {
    public:
        D3D11DefaultColorAttachment(const ColorAttachmentDesc& desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain);

        ~D3D11DefaultColorAttachment() override;

        void bind() override;

        void bindShaderResource(u32 slot) override;

        void clear() override;

        void onResize(u32 width, u32 height) override;

        [[nodiscard]] UP<Image> getData() override;

        [[nodiscard]] u64 getIdentifier() const override;

        [[nodiscard]] ID3D11RenderTargetView *getRenderTargetView() const;

    private:

        void createResources();

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;
        IDXGISwapChain* m_SwapChain;

        ID3D11RenderTargetView* m_RenderTargetView;

    };

}

#endif