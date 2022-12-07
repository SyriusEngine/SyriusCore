#pragma once

#include "../../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "D3D11Utils.hpp"
#include "D3D11ColorAttachment.hpp"
#include "D3D11DepthStencilAttachment.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11FrameBuffer: public FrameBuffer{
    public:
        D3D11FrameBuffer(const FrameBufferDesc& desc, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

        ~D3D11FrameBuffer() override;

        void bind() override;

        void unbind() override;

        void clear() override;

        void setPosition(int32 xPos, int32 yPos) override;

        void setSize(uint32 width, uint32 height) override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_DeviceContext;

        D3D11_VIEWPORT m_Viewport;

        std::vector<ID3D11RenderTargetView*> m_RenderTargetViews;
        std::vector<ID3D11RenderTargetView*> m_NullableRenderTargetViews;
        std::vector<ID3D11ShaderResourceView*> m_Nullable;
        ID3D11DepthStencilView* m_DepthStencilView;

    };

    class D3D11DefaultFrameBuffer: public FrameBuffer{
    public:
        D3D11DefaultFrameBuffer(const FrameBufferDesc& desc, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain);

        ~D3D11DefaultFrameBuffer() override;

        void bind() override;

        void unbind() override;

        void clear() override;

        void setPosition(int32 xPos, int32 yPos) override;

        void setSize(uint32 width, uint32 height) override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_DeviceContext;
        IDXGISwapChain* m_SwapChain;

        D3D11_VIEWPORT m_Viewport;

        ID3D11DepthStencilView* m_DepthStencilView;

        D3D11BackBufferColorAttachment* m_BackRenderTarget;
        ID3D11RenderTargetView* m_BackRenderTargetView;

    };

}

#endif
