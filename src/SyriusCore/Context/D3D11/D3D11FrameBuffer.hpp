#pragma once

#include "../../../../include/SyriusCore/Context/FrameBuffer.hpp"
#include "D3D11Utils.hpp"
#include "D3D11ColorAttachment.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11FrameBuffer: public FrameBuffer{
    public:
        D3D11FrameBuffer(const FrameBufferDesc& desc, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

        ~D3D11FrameBuffer() override;

        void bind() override;

        void unbind() override;

        void setClearColor(float red, float green, float blue, float alpha) override;

        void setPosition(int32 xPos, int32 yPos) override;

        void setDepthFunc(SR_DEPTH_FUNC func) override;

        void onResize(uint32 width, uint32 height) override;

        void enableDepthTest() override;

        void disableDepthTest() override;

        void clear() override;

    private:

    };

    class D3D11DefaultFrameBuffer: public FrameBuffer{
    public:
        D3D11DefaultFrameBuffer(const FrameBufferDesc& desc, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain);

        ~D3D11DefaultFrameBuffer() override;

        void bind() override;

        void unbind() override;

        void setClearColor(float red, float green, float blue, float alpha) override;

        void setPosition(int32 xPos, int32 yPos) override;

        void setDepthFunc(SR_DEPTH_FUNC func) override;

        void onResize(uint32 width, uint32 height) override;

        void enableDepthTest() override;

        void disableDepthTest() override;

        void clear() override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_DeviceContext;
        IDXGISwapChain* m_SwapChain;

        D3D11_VIEWPORT m_Viewport;

        ID3D11RenderTargetView* m_BackRenderTarget;

    };

}

#endif
