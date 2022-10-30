#pragma once

#include "D3D11Utils.hpp"
#include "../../../../include/SyriusCore/Context/DepthStencilAttachment.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{
    
    class D3D11DepthStencilAttachment: public DepthStencilAttachment{
    public:
        D3D11DepthStencilAttachment(const DepthStencilAttachmentDesc& desc, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
        
        ~D3D11DepthStencilAttachment() override;

        void bind() override;

        void bindAsTexture(uint32 slot) override;

        void unbind() override;

        void clear() override;

        void onResize(uint32 width, uint32 height) override;

        void setDepthFunc(SR_COMPARISON_FUNC func) override;

        void setDepthBufferReadOnly(bool readOnly) override;

        void setStencilFunc(SR_COMPARISON_FUNC func) override;

        void setStencilBufferReadOnly(bool readOnly) override;

        void setStencilMask(uint32 mask) override;

        [[nodiscard]] uint64 getIdentifier() override;
        
        [[nodiscard]] ID3D11DepthStencilView* getDepthStencilView() const;

        [[nodiscard]] ID3D11DepthStencilState* getDepthStencilState() const;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;
        
        ID3D11Texture2D* m_DepthStencilBuffer;
        ID3D11DepthStencilState* m_DepthStencilState;
        ID3D11ShaderResourceView* m_BufferView;
        ID3D11DepthStencilView* m_DepthStencilView;

        D3D11_CLEAR_FLAG m_ClearFlag;
    };
    
}

#endif