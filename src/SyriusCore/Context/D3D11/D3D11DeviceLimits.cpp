#include "D3D11DeviceLimits.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11DeviceLimits::D3D11DeviceLimits(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const D3D_FEATURE_LEVEL& featureLevel) :
    m_device(device),
    m_deviceContext(deviceContext),
    m_featureLevel(featureLevel) {
    }

    std::string D3D11DeviceLimits::getAPIVersion() const {
        switch (m_featureLevel) {
            case D3D_FEATURE_LEVEL_9_1:         return "D3D_FEATURE_LEVEL_9_1";
            case D3D_FEATURE_LEVEL_9_2:         return "D3D_FEATURE_LEVEL_9_2";
            case D3D_FEATURE_LEVEL_9_3:         return "D3D_FEATURE_LEVEL_9_3";
            case D3D_FEATURE_LEVEL_10_0:        return "D3D_FEATURE_LEVEL_10_0";
            case D3D_FEATURE_LEVEL_10_1:        return "D3D_FEATURE_LEVEL_10_1";
            case D3D_FEATURE_LEVEL_11_0:        return "D3D_FEATURE_LEVEL_11_0";
            case D3D_FEATURE_LEVEL_11_1:        return "D3D_FEATURE_LEVEL_11_1";
            case D3D_FEATURE_LEVEL_12_0:        return "D3D_FEATURE_LEVEL_12_0";
            case D3D_FEATURE_LEVEL_12_1:        return "D3D_FEATURE_LEVEL_12_1";
            default:                            return "Unknown";
        }
    }

    std::string D3D11DeviceLimits::getDeviceName() const {
        return {};
    }

    std::string D3D11DeviceLimits::getDeviceVendor() const {
        return {};
    }

    std::string D3D11DeviceLimits::getShadingLanguageVersion() const {
        return "HLSL 5.0";
    }

    uint32 D3D11DeviceLimits::getMaxFramebufferWidth() const {
        return D3D11_REQ_RENDER_TO_BUFFER_WINDOW_WIDTH;
    }

    uint32 D3D11DeviceLimits::getMaxFramebufferHeight() const {
        return D3D11_REQ_RENDER_TO_BUFFER_WINDOW_WIDTH;
    }

    uint32 D3D11DeviceLimits::getMaxFramebufferColorAttachments() const {
        return D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
    }

    uint32 D3D11DeviceLimits::getMaxViewportWidth() const {
        return D3D11_VIEWPORT_BOUNDS_MAX;
    }

    uint32 D3D11DeviceLimits::getMaxViewportHeight() const {
        return D3D11_VIEWPORT_BOUNDS_MAX;
    }

    uint32 D3D11DeviceLimits::getMaxTextureSlots() const {
        return D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT;
    }

    uint32 D3D11DeviceLimits::getMaxTextureSize() const {
        return D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
    }

    uint32 D3D11DeviceLimits::getMaxShaderStorageBufferSlots() const {
        return D3D11_PS_CS_UAV_REGISTER_COUNT;
    }

    uint32 D3D11DeviceLimits::getMaxConstantBufferSize() const {
        return 65536; // https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-limits
    }

    uint32 D3D11DeviceLimits::getMaxConstantBufferSlots() const {
        return D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT;
    }

    uint32 D3D11DeviceLimits::getMaxDepthBufferBits() const {
        return 32;
    }

    uint32 D3D11DeviceLimits::getMaxVertexAttributes() const {
        return 32; // https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-limits
    }

    uint64 D3D11DeviceLimits::getMaxVertexCount() const {
        auto maxVertexCount = std::pow(2, D3D11_REQ_DRAW_VERTEX_COUNT_2_TO_EXP);
        return static_cast<uint64>(maxVertexCount);
    }

    uint64 D3D11DeviceLimits::getMaxIndexCount() const {
        auto maxIndexCount =  std::pow(2, D3D11_REQ_DRAWINDEXED_INDEX_COUNT_2_TO_EXP);
        return static_cast<uint64>(maxIndexCount);
    }

    uint64 D3D11DeviceLimits::getMaxIndex() const {
        return 0;
    }

    bool D3D11DeviceLimits::isDoubleBuffered() const {
        return true;
    }


}

#endif