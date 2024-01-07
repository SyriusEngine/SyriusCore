#pragma once

#include "../../../../include/SyriusCore/Context/DeviceLimits.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlDeviceLimits: public DeviceLimits{
    public:
        GlDeviceLimits() = default;

        ~GlDeviceLimits() override = default;

        std::string getAPIVersion() override;

        std::string getDeviceName() override;

        std::string getDeviceVendor() override;

        std::string getShadingLanguageVersion() override;

        uint32 getMaxFramebufferWidth() override;

        uint32 getMaxFramebufferHeight() override;

        uint32 getMaxFramebufferColorAttachments() override;

        uint32 getMaxViewportWidth() override;

        uint32 getMaxViewportHeight() override;

        uint32 getMaxTextureSlots() override;

        uint32 getMaxTextureSize() override;

        uint32 getMaxSSSBOSlots() override;

        uint32 getMaxConstantBufferSize() override;

        uint32 getMaxConstantBufferSlots() override;

        uint32 getMaxDepthBufferBits() override;

        uint32 getMaxVertexAttributes() override;

        uint32 getMaxVertices() override; // maximum number of vertices for a draw call

        uint32 getMaxIndices() override; // maximum number of indices for a draw call

        uint32 getMaxIndex() override; // largest index value for a draw call

        bool isDoubleBuffered() override;

    private:

        static std::string getGlConstantStr(GLenum constant);

        static uint32 getGlConstantInt(GLenum constant);


    };

}