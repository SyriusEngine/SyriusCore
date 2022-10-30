#pragma once

#include "../../../../include/SyriusCore/Context/ColorAttachment.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11ColorAttachment: public ColorAttachment{
    public:
        D3D11ColorAttachment(const ColorAttachmentDesc& desc, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

        ~D3D11ColorAttachment() override;

        void bind(uint32 slot) override;

        [[nodiscard]] Image* getData() override;

        void onResize(uint32 width, uint32 height) override;

        [[nodiscard]] uint64 getIdentifier() const override;

        [[nodiscard]] ID3D11RenderTargetView* getRenderTargetView() const;

    private:

        DXGI_FORMAT getFormat(uint32 channelCount);

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11Texture2D* m_ColorBuffer;
        ID3D11RenderTargetView* m_RenderTargetView;
        ID3D11ShaderResourceView* m_BufferView;

    };

}

#endif