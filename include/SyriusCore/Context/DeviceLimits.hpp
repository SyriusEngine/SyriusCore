#pragma once

#include "Utils.hpp"

namespace Syrius{

    class SR_CORE_API DeviceLimits{
    public:
        DeviceLimits() = default;

        virtual ~DeviceLimits() = default;

        virtual std::string getAPIVersion() = 0;

        virtual std::string getDeviceName() = 0;

        virtual std::string getDeviceVendor() = 0;

        virtual std::string getShadingLanguageVersion() = 0;

        virtual uint32 getMaxFramebufferWidth() = 0;

        virtual uint32 getMaxFramebufferHeight() = 0;

        virtual uint32 getMaxFramebufferColorAttachments() = 0;

        virtual uint32 getMaxViewportWidth() = 0;

        virtual uint32 getMaxViewportHeight() = 0;
        
        virtual uint32 getMaxTextureSlots() = 0;
        
        virtual uint32 getMaxTextureSize() = 0;

        virtual uint32 getMaxSSSBOSlots() = 0;
        
        virtual uint32 getMaxConstantBufferSize() = 0;

        virtual uint32 getMaxConstantBufferSlots() = 0;
        
        virtual uint32 getMaxDepthBufferBits() = 0;
        
        virtual uint32 getMaxVertexAttributes() = 0;

        virtual uint32 getMaxVertices() = 0; // maximum number of vertices for a draw call

        virtual uint32 getMaxIndices() = 0; // maximum number of indices for a draw call

        virtual uint32 getMaxIndex() = 0; // largest index value for a draw call

        virtual bool isDoubleBuffered() = 0;

    };

}