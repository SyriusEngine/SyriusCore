#pragma once

#include "../../../../include/SyriusCore/Context/DepthStencilAttachment.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11DepthStencilAttachment : public DepthStencilAttachment{
    public:
        D3D11DepthStencilAttachment(const DepthStencilAttachmentDesc& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

        ~D3D11DepthStencilAttachment() override;

        void bind() override;

        void unbind() override;

        void bindShaderResource(uint32 slot) override;

        void clear() override;

        void onResize(uint32 width, uint32 height) override;

        void enableDepthTest(bool enable) override;

        void setDepthFunc(SR_COMPARISON_FUNC func) override;

        void setDepthMask(SR_DEPTH_MASK mask) override;

        [[nodiscard]] Resource<Image> getData() override;

        [[nodiscard]] uint64 getIdentifier() const override;

        [[nodiscard]] ID3D11DepthStencilView* getDepthStencilView() const;

        [[nodiscard]] ID3D11DepthStencilState* getDepthStencilState() const;

    private:

        void setClearFlag();

        void createState();

        void createBufferAndView();

        void destroyState();

        void destroyBufferAndView();

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11Texture2D* m_Buffer;
        ID3D11DepthStencilState* m_State;
        ID3D11DepthStencilView* m_View;
        ID3D11ShaderResourceView* m_ShaderResourceView;

        D3D11_CLEAR_FLAG m_ClearFlag;
    };

}

#endif