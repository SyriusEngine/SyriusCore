#pragma once

#include "../../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "D3D11Utils.hpp"
#include "D3D11Viewport.hpp"
#include "D3D11ColorAttachment.hpp"
#include "D3D11DepthStencilAttachment.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11FrameBuffer : public FrameBuffer{
    public:
        D3D11FrameBuffer(const ResourceView<FrameBufferDescription>& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

        ~D3D11FrameBuffer() override;

        void bind() override;

        void unbind() override;

        void onResize(uint32 width, uint32 height) override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_DeviceContext;

        std::vector<D3D11ColorAttachment*> m_D3D11ColorAttachments;
        std::vector<ID3D11RenderTargetView*> m_RenderTargetViews;
        std::vector<ID3D11RenderTargetView*> m_NullableRenderTargetViews;
        std::vector<ID3D11ShaderResourceView*> m_NullableShaderResourceViews;

        ID3D11DepthStencilView* m_DepthStencilView;
        D3D11DepthStencilAttachment* m_D3D11DepthStencilAttachment;
    };

    class D3D11DefaultFrameBuffer : public FrameBuffer{
    public:
        D3D11DefaultFrameBuffer(const ResourceView<FrameBufferDescription>& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain);

        ~D3D11DefaultFrameBuffer() override;

        void bind() override;

        void unbind() override;

        void onResize(uint32 width, uint32 height) override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_DeviceContext;
        IDXGISwapChain* m_SwapChain;

        ID3D11DepthStencilView* m_DepthStencilView;
        D3D11DepthStencilAttachment* m_D3D11DepthStencilAttachment;

        D3D11DefaultColorAttachment* m_ColorAttachment;
        ID3D11RenderTargetView* m_RenderTargetView;
    };

}

#endif