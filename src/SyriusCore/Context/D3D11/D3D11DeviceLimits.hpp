#pragma once

#include "../../../../include/SyriusCore/Context/DeviceLimits.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11DeviceLimits: public DeviceLimits {
    public:
        D3D11DeviceLimits(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const D3D_FEATURE_LEVEL& featureLevel);

        ~D3D11DeviceLimits() override = default;

        [[nodiscard]] std::string getAPIVersion() const override;

        [[nodiscard]] std::string getDeviceName() const override;

        [[nodiscard]] std::string getDeviceVendor() const override;

        [[nodiscard]] std::string getShadingLanguageVersion() const override;

        [[nodiscard]] u32 getMaxFramebufferWidth() const override;

        [[nodiscard]] u32 getMaxFramebufferHeight() const override;

        [[nodiscard]] u32 getMaxFramebufferColorAttachments() const override;

        [[nodiscard]] u32 getMaxViewportWidth() const override;

        [[nodiscard]] u32 getMaxViewportHeight() const override;

        [[nodiscard]] u32 getMaxTextureSlots() const override;

        [[nodiscard]] u32 getMaxTextureSize() const override;

        [[nodiscard]] u32 getMaxConstantBufferSize() const override;

        [[nodiscard]] u32 getMaxConstantBufferSlots() const override;

        [[nodiscard]] u32 getMaxShaderStorageBufferSize() const override;

        [[nodiscard]] u32 getMaxShaderStorageBufferSlots() const override;

        [[nodiscard]] u32 getMaxDepthBufferBits() const override;

        [[nodiscard]] u32 getMaxVertexAttributes() const override;

        [[nodiscard]] u64 getMaxVertexCount() const override; // maximum number of vertices for a draw call

        [[nodiscard]] u64 getMaxIndexCount() const override; // maximum number of indices for a draw call

        [[nodiscard]] u64 getMaxIndex() const override; // largest index value for a draw call

        [[nodiscard]] bool isDoubleBuffered() const override;

        [[nodiscard]] bool texture2DFormatSupported(SR_TEXTURE_FORMAT format) override;

    private:
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_deviceContext;
        const D3D_FEATURE_LEVEL& m_featureLevel;
    };


}

#endif