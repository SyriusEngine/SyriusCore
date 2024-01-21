#pragma once

#include "../../../../include/SyriusCore/Context/ColorAttachment.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11ColorAttachment : public ColorAttachment {
    public:
        D3D11ColorAttachment(const ColorAttachmentDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *deviceContext);

        ~D3D11ColorAttachment() override;

        void bind() override;

        void bindShaderResource(uint32 slot) override;

        void clear() override;

        void onResize(uint32 width, uint32 height) override;

        [[nodiscard]] Resource<Image> getData() override;

        [[nodiscard]] uint64 getIdentifier() const override;

        [[nodiscard]] ID3D11RenderTargetView *getRenderTargetView() const;

    private:

        void createResources();

        void destroyResources();

    private:
        ID3D11Device *m_Device;
        ID3D11DeviceContext *m_Context;

        ID3D11Texture2D *m_ColorBuffer;
        ID3D11RenderTargetView *m_RenderTargetView;
        ID3D11ShaderResourceView *m_BufferView;
    };

    class D3D11DefaultColorAttachment : public ColorAttachment {
    public:
        D3D11DefaultColorAttachment(const ColorAttachmentDesc& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain);

        ~D3D11DefaultColorAttachment() override;

        void bind() override;

        void bindShaderResource(uint32 slot) override;

        void clear() override;

        void onResize(uint32 width, uint32 height) override;

        [[nodiscard]] Resource<Image> getData() override;

        [[nodiscard]] uint64 getIdentifier() const override;

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