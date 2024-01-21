#pragma once

#include "../../../../include/SyriusCore/Context/DeviceLimits.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    class D3D11DeviceLimits: public DeviceLimits {
    public:
        D3D11DeviceLimits(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const D3D_FEATURE_LEVEL& featureLevel);

        ~D3D11DeviceLimits() override = default;

        std::string getAPIVersion() const override;

        std::string getDeviceName() const override;

        std::string getDeviceVendor() const override;

        std::string getShadingLanguageVersion() const override;

        uint32 getMaxFramebufferWidth() const override;

        uint32 getMaxFramebufferHeight() const override;

        uint32 getMaxFramebufferColorAttachments() const override;

        uint32 getMaxViewportWidth() const override;

        uint32 getMaxViewportHeight() const override;

        uint32 getMaxTextureSlots() const override;

        uint32 getMaxTextureSize() const override;

        uint32 getMaxSSSBOSlots() const override;

        uint32 getMaxConstantBufferSize() const override;

        uint32 getMaxConstantBufferSlots() const override;

        uint32 getMaxDepthBufferBits() const override;

        uint32 getMaxVertexAttributes() const override;

        uint64 getMaxVertexCount() const override; // maximum number of vertices for a draw call

        uint64 getMaxIndexCount() const override; // maximum number of indices for a draw call

        uint64 getMaxIndex() const override; // largest index value for a draw call

        bool isDoubleBuffered() const override;

    private:
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_deviceContext;
        const D3D_FEATURE_LEVEL& m_featureLevel;
    };


}

#endif