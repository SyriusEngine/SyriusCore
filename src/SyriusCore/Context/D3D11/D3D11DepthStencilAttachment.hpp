#pragma once

#include "../../../../include/SyriusCore/Context/DepthStencilAttachment.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11DepthStencilAttachment : public DepthStencilAttachment{
    public:
        explicit D3D11DepthStencilAttachment(const DepthStencilAttachmentDesc& desc, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

        ~D3D11DepthStencilAttachment() override;

        void bind() override;

        void unbind() override;

        void bindShaderResource(uint32 slot) override;

        void clear() override;

        void setSize(uint32 width, uint32 height) override;

        void enableDepthTest(bool enable) override;

        [[nodiscard]] Resource<Image> getData() override;

        [[nodiscard]] uint64 getIdentifier() const override;

        [[nodiscard]] ID3D11DepthStencilView* getDepthStencilView() const;

        [[nodiscard]] ID3D11DepthStencilState* getDepthStencilState() const;

    private:

        void setClearFlag();

        void createDepthStencilState();

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11Texture2D* m_DepthStencilBuffer;
        ID3D11DepthStencilState* m_DepthStencilState;
        ID3D11DepthStencilView* m_DepthStencilView;

        D3D11_CLEAR_FLAG m_ClearFlag;

        DXGI_FORMAT m_D3DFormat;
    };

}

#endif