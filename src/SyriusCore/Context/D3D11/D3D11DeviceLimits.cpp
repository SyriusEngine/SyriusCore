#include "D3D11DeviceLimits.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11DeviceLimits::D3D11DeviceLimits(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const D3D_FEATURE_LEVEL& featureLevel)
        : m_device(device), m_deviceContext(deviceContext), m_featureLevel(featureLevel) {
    }

    std::string D3D11DeviceLimits::getAPIVersion() {
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

    std::string D3D11DeviceLimits::getDeviceName() {
        return {};
    }

    std::string D3D11DeviceLimits::getDeviceVendor() {
        return {};
    }

    std::string D3D11DeviceLimits::getShadingLanguageVersion() {
        return "HLSL 5.0";
    }

    uint32 D3D11DeviceLimits::getMaxFramebufferWidth() {
        return D3D11_REQ_RENDER_TO_BUFFER_WINDOW_WIDTH;
    }

    uint32 D3D11DeviceLimits::getMaxFramebufferHeight() {
        return D3D11_REQ_RENDER_TO_BUFFER_WINDOW_WIDTH;
    }

    uint32 D3D11DeviceLimits::getMaxFramebufferColorAttachments() {
        return D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
    }

    uint32 D3D11DeviceLimits::getMaxViewportWidth() {
        return D3D11_VIEWPORT_BOUNDS_MAX;
    }

    uint32 D3D11DeviceLimits::getMaxViewportHeight() {
        return D3D11_VIEWPORT_BOUNDS_MAX;
    }

    uint32 D3D11DeviceLimits::getMaxTextureSlots() {
        return D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT;
    }

    uint32 D3D11DeviceLimits::getMaxTextureSize() {
        return D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
    }

    uint32 D3D11DeviceLimits::getMaxSSSBOSlots() {
        return D3D11_PS_CS_UAV_REGISTER_COUNT;
    }

    uint32 D3D11DeviceLimits::getMaxConstantBufferSize() {
        return 65536; // https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-limits
    }

    uint32 D3D11DeviceLimits::getMaxConstantBufferSlots() {
        return D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT;
    }

    uint32 D3D11DeviceLimits::getMaxDepthBufferBits() {
        return D3D11_PS_OUTPUT_DEPTH_REGISTER_COUNT;
    }

    uint32 D3D11DeviceLimits::getMaxVertexAttributes() {
        return 32; // https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-limits
    }

    uint32 D3D11DeviceLimits::getMaxVertices() {
        return std::pow(2, D3D11_REQ_DRAW_VERTEX_COUNT_2_TO_EXP);
    }

    uint32 D3D11DeviceLimits::getMaxIndices() {
        return std::pow(2, D3D11_REQ_DRAWINDEXED_INDEX_COUNT_2_TO_EXP);
    }

    uint32 D3D11DeviceLimits::getMaxIndex() {
        return 0;
    }

    bool D3D11DeviceLimits::isDoubleBuffered() {
        return true;
    }


}

#endif