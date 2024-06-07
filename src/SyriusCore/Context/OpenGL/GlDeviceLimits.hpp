#pragma once

#include "../../../../include/SyriusCore/Context/DeviceLimits.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlDeviceLimits: public DeviceLimits{
    public:
        GlDeviceLimits() = default;

        ~GlDeviceLimits() override = default;

        [[nodiscard]] std::string getAPIVersion() const override;

        [[nodiscard]] std::string getDeviceName() const override;

        [[nodiscard]] std::string getDeviceVendor() const override;

        [[nodiscard]] std::string getShadingLanguageVersion() const override;

        [[nodiscard]] uint32 getMaxFramebufferWidth() const override;

        [[nodiscard]] uint32 getMaxFramebufferHeight() const override;

        [[nodiscard]] uint32 getMaxFramebufferColorAttachments() const override;

        [[nodiscard]] uint32 getMaxViewportWidth() const override;

        [[nodiscard]] uint32 getMaxViewportHeight() const override;

        [[nodiscard]] uint32 getMaxTextureSlots() const override;

        [[nodiscard]] uint32 getMaxTextureSize() const override;

        [[nodiscard]] uint32 getMaxSSSBOSlots() const override;

        [[nodiscard]] uint32 getMaxConstantBufferSize() const override;

        [[nodiscard]] uint32 getMaxConstantBufferSlots() const override;

        [[nodiscard]] uint32 getMaxDepthBufferBits() const override;

        [[nodiscard]] uint32 getMaxVertexAttributes() const override;

        [[nodiscard]] uint64 getMaxVertexCount() const override; // maximum number of vertices for a draw call

        [[nodiscard]] uint64 getMaxIndexCount() const override; // maximum number of indices for a draw call

        [[nodiscard]] uint64 getMaxIndex() const override; // largest index value for a draw call

        [[nodiscard]] bool isDoubleBuffered() const override;

        [[nodiscard]] bool texture2DFormatSupported(SR_TEXTURE_FORMAT format) override;

    private:

        static std::string getGlConstantStr(GLenum constant);

        static uint32 getGlConstantInt(GLenum constant);

    };

}